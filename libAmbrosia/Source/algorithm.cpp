/**
 *
 * Project Ambrosia: Ambrosia library
 *
 * Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related
 * and neighboring rights to this software to the public domain worldwide. This software is
 * distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Ambrosia/algorithm.cpp
 * Function implementations.
 *
 **/

// Function include
#include "Ambrosia/algorithm.h"

// libAmbrosia includes
#include "Ambrosia/build_element.h"
#include "Ambrosia/configuration.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/Error/nectar_error.h"
#include "Ambrosia/platform.h"
//#include "Ambrosia/project.h"
//#include "Ambrosia/target.h"

// C++ includes
#include <istream>
  using std::istream;
#include <iterator>
  using std::insert_iterator;
#include <map>
  using std::map;
#include <memory>
  using std::unique_ptr;
#include <set>
  using std::set;
#include <string>
  using std::string;
#include <utility>
  using std::pair;
#include <vector>
  using std::vector;

// C-ish includes
#include <cstring>

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
size_t s_full_directory_name_calls = 0;
#endif

/* Freestanding functions
 *************************/
const std::string full_directory_name(const string& first_directory,
                                      const string& second_directory )
{
#ifdef AMBROSIA_DEBUG
  ++s_full_directory_name_calls;
#endif
  if(first_directory.empty())
    return second_directory;
  else if(second_directory.empty())
    return first_directory;
  else
    return first_directory + "/" + second_directory;
}

bool wildcard_compare(const string& wild_string,
                      const string& full_string)
{
  // Written by Jack Handy - jackhandy@hotmail.com
  // Taken from http://www.codeproject.com/KB/string/wildcmp.aspx
  // Adapted by Ruben Van Boxem for Ambrosia

  auto&& wild = std::begin(wild_string);
  auto&& str = std::begin(full_string);
  const auto&& wild_end = std::end(wild_string);
  const auto&& string_end = std::end(full_string);

  auto cp = string_end;
  auto mp = wild_end;

  while(str != string_end && (*wild != '*'))
  {
    if((*wild != *str) && (*wild != '?'))
      return false;

    ++wild;
    ++str;
  }
  while(str != string_end) // string != end
  {
    if(*wild == '*')
    {
      ++wild;
      if(wild == wild_end)
        return true;

      mp = wild;
      cp = str+1;
    }
    else if((*wild == *str) || (*wild == '?'))
    {
      ++wild;
      ++str;
    }
    else
    {
      wild = mp;
      str = cp;
      if(cp == string_end)
        return false;

      ++cp;
    }
  }
  while(wild != wild_end && *wild == '*')
  {
    ++wild;
  }
  return (wild==wild_end);
}

bool wildcard_directory_compare(const string& wild_string,
                                const string& full_string)
{
  // Written by Jack Handy - jackhandy@hotmail.com
  // Taken from http://www.codeproject.com/KB/string/wildcmp.aspx
  // Adapted by Ruben Van Boxem for Ambrosia

  auto&& wild = std::begin(wild_string);
  auto&& str = std::begin(full_string);

  auto cp = std::end(full_string);
  auto mp = std::end(wild_string);

  while(str != std::end(full_string) && (*wild != '*'))
  {
    if((*wild != *str) && (*wild != '?'))
      return false;

    ++wild;
    ++str;
  }
  while(str != std::end(full_string)) // string != end
  {
    if(*wild == '*')
    {
      ++wild;
      if(wild == std::end(wild_string))
        return true;

      mp = wild;
      cp = str+1;
    }
    else if((*wild == *str) || (*wild == '?'))
    {
      ++wild;
      ++str;
    }
    else
    {
      wild = mp;
      str = cp;
      if(*str == '/')
        return false;

      ++cp;
    }
  }
  while(wild != std::end(wild_string) && *wild == '*')
  {
    ++wild;
  }
  return (wild==std::end(wild_string));
}

template<class T>
const T merge_sets(T& old_set,
                   const T& add_set)
{
  // TODO: optimize
  T result;
  T duplicates;
  // get merged set
  std::set_union(std::begin(old_set), std::end(old_set),
                 std::begin(add_set), std::end(add_set),
                 std::inserter(result, std::end(result)));
  // get duplicates
  std::set_intersection(std::begin(old_set), std::end(old_set),
                        std::begin(add_set), std::end(add_set),
                        std::inserter(duplicates, std::end(duplicates)));
  old_set.swap(result);
  // return duplicates for error handling
  return duplicates;
}
template const string_set merge_sets<string_set>(string_set&, const string_set&);

template<class T>
const T remove_set(T& old_set,
                   const T& new_set)
{
  // TODO: optimize
  T result;
  T not_found;

  // find elements in new_set that are not in old_set
  std::set_symmetric_difference(std::begin(new_set), std::end(new_set),
                                std::begin(old_set), std::end(old_set),
                                std::inserter(not_found, std::end(not_found)));

  // remove elements in new_set
  std::set_difference(std::begin(old_set), std::end(old_set),
                      std::begin(new_set), std::end(new_set),
                      std::inserter(result, std::begin(result)));
  old_set.swap(result);
  // return items not present in old_set
  return not_found;
}
template const string_set remove_set<string_set>(string_set&, const string_set&);

/*
 * libAmbrosia dependent functions
 **********************************/
const string replace_directory_seperators(const string& original)
{
  //TODO: use std::relace_if
  if('/' == directory_seperator)
  {
    debug(debug::algorithm) << "algorithm::replace_directory_seperators::Forward slash is directory_seperator.\n";
    return original;
  }

  string replaced;
  replaced.resize(original.size());
  for(string::size_type i = 0; i < original.size(); ++i)
  {
    if('/' == original[i])
      replaced[i] = directory_seperator;
    else
      replaced[i] = original[i];
  }
  return replaced;
}
const string_pair split_preceding_directory(const string& path)
{
  const string::size_type index = path.find_last_of("/");
  if(index != string::npos)
  {
    // handle path ending in directory seperator
    if(index == path.size()-1)
      return std::make_pair("", path.substr(0,index-1));
  else
    return std::make_pair(path.substr(0, index), path.substr(index+1));
  }
  else
    return std::make_pair("", path);
}
void skip_BOM(istream& stream,
              const string& filename)
{
  const unsigned char BOM[] = {0xef, 0xbb, 0xbf};

  char first_3_chars[3];
  if(!stream.read(first_3_chars, 3))
    throw error("Unexpected end of file: " + filename + ".");

  if(memcmp(reinterpret_cast<const char*>(BOM), first_3_chars, 3))
    stream.seekg( 0, std::ios::beg ); // reset to beginning of file
}

/*
def dep_resolve(node, resolved, unresolved):
   unresolved.append(node) # skip this, already present
   for edge in node.edges:
      if edge not in resolved:
         if edge in unresolved:
            raise Exception('Circular reference detected: %s -> %s' % (node.name, edge.name))
         dep_resolve(edge, resolved, unresolved)
   resolved.append(node)
   unresolved.remove(node) # do this, clear the original target_vector
*/

libambrosia_namespace_end
