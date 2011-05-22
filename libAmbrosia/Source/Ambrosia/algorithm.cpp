/**
  * Ambrosia/algorithm.cpp
  * Function implementations.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "algorithm.h"

// libAmbrosia includes
#include "build_config.h"
#include "debug.h"
#include "status.h"
#include "target.h"

// C++ includes
#include <istream>
    using std::istream;
#include <memory>
    using std::unique_ptr;
/* <set> */
    using std::set;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

// C-ish includes
#include <cstring>

libambrosia_namespace_begin

/* Freestanding functions
 *************************/
bool wildcard_compare( const string &wild_string, const string &full_string )
{
    auto wild = wild_string.begin();
    auto str = full_string.begin();
    const auto wild_end = wild_string.end();
    const auto string_end = full_string.end();

    auto cp = string_end;
    auto mp = wild_end;

    while( str != string_end && (*wild != '*') )
    {
        if( (*wild != *str) && (*wild != '?') )
            return false;

        ++wild;
        ++str;
    }
    while( str != string_end ) // string != end
    {
        if( *wild == '*' )
        {
            ++wild;
            if( wild == wild_end )
                return true;

            mp = wild;
            cp = str+1;
        }
        else if( (*wild == *str) || (*wild == '?') )
        {
            ++wild;
            ++str;
        }
        else
        {
            wild = mp;
            str = cp;
            ++cp;
        }
    }
    while( *wild == '*' )
    {
        ++wild;
    }
    return ( wild==wild_end );
}

bool wildcard_directory_compare( const string &wild_string, const string &full_string )
{
    // Written by Jack Handy - jakkhandy@hotmail.com
    // Taken from http://www.codeproject.com/KB/string/wildcmp.aspx
    // Adapted by Ruben Van Boxem for Ambrosia

    auto wild = wild_string.begin();
    auto str = full_string.begin();
    const auto wild_end = wild_string.end();
    const auto string_end = full_string.end();

    auto cp = string_end;
    auto mp = wild_end;

    while( str != string_end && (*wild != '*') )
    {
        if( (*wild != *str) && (*wild != '?') )
            return false;

        ++wild;
        ++str;
    }
    while( str != string_end ) // string != end
    {
        if( *wild == '*' )
        {
            ++wild;
            if( wild == wild_end )
                return true;

            mp = wild;
            cp = str+1;
        }
        else if( (*wild == *str) || (*wild == '?') )
        {
            ++wild;
            ++str;
        }
        else
        {
            wild = mp;
            str = cp;
            if( *str == '/' )
                return false;

            ++cp;
        }
    }
    while( wild != wild_end && *wild == '*' )
    {
        ++wild;
    }
    return ( wild==wild_end );
}
void filter_duplicates( set<string> &unfiltered, const set<string> &reference,
                        set<string> &duplicates )
{
    set_intersection( unfiltered.begin(), unfiltered.end(),
                      reference.begin(), reference.end(),
                      inserter(duplicates, duplicates.begin()) );
    set<string> difference;
    set_difference( unfiltered.begin(), unfiltered.end(),
                    duplicates.begin(), duplicates.end(),
                    inserter(difference, difference.begin()) );
    unfiltered = difference;
}
/* libAmbrosia dependent functions
 **********************************/
void skip_BOM( istream &stream )
{
    const unsigned char BOM[] = { 0xef, 0xbb, 0xbf };

    char first_3_chars[3];
    if( !stream.read( first_3_chars, 3 ) )
        return emit_error( "Unexpected end of file." );

    if( memcmp(reinterpret_cast<const char*>(BOM), first_3_chars, 3) )
        stream.seekg( 0, std::ios::beg ); // reset to beginning of file
}

/*
def dep_resolve(node, resolved, unresolved):
   unresolved.append(node)
   for edge in node.edges:
      if edge not in resolved:
         if edge in unresolved:
            raise Exception('Circular reference detected: %s -> %s' % (node.name, edge.name))
         dep_resolve(edge, resolved, unresolved)
   resolved.append(node)
   unresolved.remove(node)
*/
void dependency_resolve( target_list &unsorted, target_list::iterator node,
                         target_list &resolved, target_list &unresolved )
{
    debug(6) << "dependency_resolve::Resolving: " << (*node)->name() << ".\n";
    unresolved.push_back( std::move(*node) );
    unsorted.erase( unsorted.begin() );

    const auto edges = unresolved.back()->dependencies();
    const auto end = edges.end();
    for( auto it = edges.begin(); it != end; ++it )
    {
        const string name( (*it).second );
        debug(6) << "dependency_resolve::Processing edge: " << name << ".\n";
        const auto find_functor = [&name](const target_list::value_type &t)
                                  {   return name == t->name();   };

        if( resolved.end() == find_if(resolved.begin(), resolved.end(),
                                      find_functor) )
        {
            if( unresolved.end() != find_if(unresolved.begin(), unresolved.end(),
                                          find_functor) )
                return emit_error( "Circular dependency detected: " + unresolved.back()->name() + " -> " + name + "." );

            // check if dependency is already resolved or still needs to be processed
            auto new_node = std::find_if( unsorted.begin(), unsorted.end(),
                                          find_functor );
            if( new_node != unsorted.end() )
                dependency_resolve( unsorted, new_node,
                                    resolved, unresolved );
            else
            {
                new_node = std::find_if( resolved.begin(), resolved.end(),
                                         find_functor );
                if( new_node == resolved.end() )
                    return emit_error( "Dependency not defined: " + name );
            }
        }
    }
    resolved.push_back( std::move(unresolved.back()) );
    unresolved.erase( unresolved.end()-1 );
}

void dependency_sort( target_list &unsorted )
{
    target_list resolved;
    target_list unresolved;
    resolved.reserve( unsorted.size() );
    unresolved.reserve( unsorted.size() );

    while( !unsorted.empty() )
    {
        dependency_resolve( unsorted, unsorted.begin(),
                            resolved, unresolved );
    }
    if( error_status() )
        return;

    unsorted.swap(resolved);
}

void filter_dependency_sort( target_list &unsorted )
{
    target_list resolved;
    target_list unresolved;
    resolved.reserve( unsorted.size() );
    unresolved.reserve( unsorted.size() );

    const auto &target_config = s_build_config.target_config();
    const auto end = target_config.end();
    for( auto it = target_config.begin(); it != end; ++it )
    {
        const string name( (*it).first );
        const auto item = std::find_if( unsorted.begin(), unsorted.end(),
                                        [&name](const unique_ptr<target> &t) {return name == t->name();} );
        if( item == unsorted.end() )
        {
            unsorted.erase( item );
            continue; // skip dependency_resolve, of course
        }
        dependency_resolve( unsorted, unsorted.begin(),
                            resolved, unresolved );
    }
    if( error_status() )
        return;

    unsorted.swap(resolved);
}
const string_set find_matching_files( const std::string &filename, const string_set &directories,
                                      const file_set &files )
{

}

libambrosia_namespace_end
