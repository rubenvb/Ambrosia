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
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/Error/nectar_error.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/Targets/project.h"

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

/* Freestanding functions
 *************************/
const string operator /(const string& left, const string& right)
{
  if(left.empty())
    return right;
  else if(right.empty())
    return left;
  else if('/' == left.back())
    return left.substr(0,left.size()-2) + "/" + right;
  else
    return left + "/" + right;
}

bool wildcard_compare(const string& wild_string,
                      const string& full_string)
{
  // Written by Jack Handy - jackhandy@hotmail.com
  // Taken from http://www.codeproject.com/KB/string/wildcmp.aspx
  // Adapted by Ruben Van Boxem for Ambrosia

  auto wild = std::begin(wild_string);
  auto str = std::begin(full_string);
  const auto wild_end = std::end(wild_string);
  const auto string_end = std::end(full_string);

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

  auto wild = std::begin(wild_string);
  auto str = std::begin(full_string);

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
  //TODO: use std::replace_if
  if('/' == platform::directory_seperator)
  {
    debug(debug::algorithm) << "algorithm::replace_directory_seperators::Forward slash is directory_seperator.\n";
    return original;
  }

  string replaced;
  replaced.resize(original.size());
  for(string::size_type i = 0; i < original.size(); ++i)
  {
    if('/' == original[i])
      replaced[i] = platform::directory_seperator;
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
void find_dependencies(const project& project,
                       const target_type type,
                       const string& name,
                       dependency_set& dependencies)
{
  //TODO: what with circular dependencies?
  debug(debug::algorithm) << "algorithm::find_dependencies::Locating dependency of type " << target_type_map_inverse.at(type) << ": " << name << ".\n";
  auto result = std::find_if(std::begin(project.dependencies), std::end(project.dependencies),[&type,&name](const dependency& dep) { return dep.name == name && (dep.type == type || dep.type == target_type::external); });
  if(result != std::end(project.dependencies))
  {
    debug(debug::algorithm) << "algorithm::find_dependencies::Found parent dependency: " << result->name << ".\n";
    if(!dependencies.insert(dependency(name, type, result->target)).second)
      throw error("Two dependency targets with the same name detected when resolving dependencies. All targets must have distinct names.");
  }
  // find other project targets that match, including searching subprojects recursively.
  for(auto&& target : project.targets)
  {
    debug(debug::algorithm) << "algorithm::find_dependencies::Checking target " << target->name << " as a possible match.\n";
    if(target->name != name && wildcard_compare("*::"+target->name, name)) // check for name and qualified name
    {
      debug(debug::algorithm) << "algorithm::find_dependencies::Apparently, " << "*"+target->name << " != " << name << "\n";
      continue; // no match, ever
    }
    else if(target->type == type)
    {
      debug(debug::algorithm) << "algorithm::find_dependencies::Located exact match: " << target->name <<" to " << name << ".\n";
      if(dependencies.insert(dependency(name, type, target.get())).second)
        throw error("Two dependency targets with the same name detected when resolving dependencies. All targets must have distinct names.");
    }
    else if(target->type == target_type::project)
    {
      debug(debug::algorithm) << "algorithm::find_dependencies::Located subproject: " << target->name << " with possible matches.\n";
      find_dependencies_in_subproject(*static_cast<libambrosia::project*>(target.get()), type, name, dependencies);
    }
    else
      debug(debug::algorithm) << "algorithm::find_dependencies::Not a match: " << target->name << ".\n";
  }
}
void find_dependencies_in_subproject(const project& project,
                                     const target_type type,
                                     const string& name,
                                     dependency_set& dependencies)
{
  debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Finding " << target_type_map_inverse.at(type) << " targets in subproject " << project.name << " matching " << name << ".\n";
  for(auto&& target : project.targets)
  {
    debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Inspecting possible dependency: " << target->name << ".\n";
    if(target->type == type)
    {
      debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Found dependency match in subproject " << project.name << ": " << target->name << " for " << name << ".\n";
      debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Adding found dependency " << target->name << " for " << name << "to the list:\n" << dependencies << "\n";
      auto result = dependencies.insert(dependency(name, type, target.get()));
      if(!result.second)
        debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Dependency already in list: " << name << ", duplicate: " << result.first->name << ".\n";

      debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Current dependencies:\n" << dependencies << "\n";
    }
    else if(target->type == target_type::project)
    {
      debug(debug::algorithm) << "algorithm::find_dependencies_in_subproject::Located subsubproject " << target->name << " with possible matches.\n";
      find_dependencies_in_subproject(project, type, name, dependencies);
    }
  }
}

libambrosia_namespace_end
