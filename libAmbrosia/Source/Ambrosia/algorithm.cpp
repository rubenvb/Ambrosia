/**
  * Ambrosia/algorithm.cpp
  * Function implementations.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "algorithm.h"
#include "target.h"

// C++ includes
/* <memory> */
    using std::unique_ptr;
/* <set> */
    using std::set;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

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
const string tokenize( const string &line, const set<char> &special_characters )
{
    const auto not_found = special_characters.end();
    const auto end = line.end();
    string result;

    if( !line.empty() )
    {
        // copy first character
        result += line[0];

        char previous = line[0];
        for( auto it = line.begin()+1; it != end; ++it )
        {
            const char current = *it;

            if( special_characters.find(previous) != not_found )
                result += ' ';

            result += current;
            previous = current;
        }
    }
    return result;
}
/* libAmbrosia dependent functions
 **********************************/
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
//template<class node>
void dependency_sort( vector<unique_ptr<target> > &unsorted )
{
    vector<unique_ptr<target> > sorted;
    sorted.reserve( unsorted.size() );

    // Go through unsorted until it is empty, moving the unique_ptr's as they are processed.
    while( unsorted.size() > 0 )
    {
        // TODO
        unsorted.erase(unsorted.begin());
    }
}
//template void dependency_sort<target>( vector<unique_ptr<target> > & );


libambrosia_namespace_end
