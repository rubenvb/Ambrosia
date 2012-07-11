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
 * Ambrosia/algorithm.h
 * Ambrosia's handy semi-generic algorithms.
 *
 **/

#ifndef AMBROSIA_ALGORITHM_H
#define AMBROSIA_ALGORITHM_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/target.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <sstream>

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
extern size_t s_full_directory_name_calls;
#endif

/* Freestanding functions (libAmbrosia independent, including no untransparent error handling)
 *************************/
// merges directory and subdirectory names to one, does the right thing if the second part is empty
const std::string full_directory_name(const std::string& first_directory,
                                      const std::string& second_directory);
// returns true if expanding '?' or '*' produces a match
bool wildcard_compare(const std::string& wildcard_string,
                      const std::string& full_string);
// returns true if expanding '?' or '*' between forward slashes '/' produces a match
bool wildcard_directory_compare(const std::string& wildcard_string,
                                const std::string& full_string);
// get filename extension
inline const std::string get_extension(const std::string& filename)
{
  const size_t index = filename.rfind('.');
  if(index!=std::string::npos)
    return filename.substr(index+1);
  else
    return "";
}

// adds items in new_set to old_set, and returns any duplicates
template<class T>
const T merge_sets(T& old_set,
                   const T& add_set);
// removes items in new_set from old_set, and returns any items not found in old_set
template<class T>
const T remove_set(T& old_set,
                   const T& remove_set);

inline bool contains(const std::string& token,
                     const std::string& characters)
{
  return (std::find_first_of(token.begin(), token.end(), characters.begin(), characters.end()) != token.end());
}
template <class K, class V>
bool contains(const std::map<K, V>& map,
              const K key)
{
  return (map.find(key) != map.end());
}
// returns true if container contains element
template <class container>
bool contains(const container& cont,
              const typename container::value_type& elem)
{
  return (std::find(cont.begin(), cont.end(), elem) != cont.end());
}

inline bool has_space(const std::string& str)
{
  return contains(str, ' ');
}
// conversion from string to numeric type
template<class T>
T from_string(const std::string& s)
{
  std::istringstream stream(s);
  T t;
  stream >> t;
  return t;
}
// conversion to string
template <class T>
const std::string to_string(const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}
template<>
inline const std::string to_string<bool>(const bool& b)
{
  return b?"true":"false";
}

// convert a newline \n character string to \\n
inline const std::string output_form(const char c)
{
  if('\n' == c)
    return std::string("\\n (newline)");
  else
    return std::string(1,c);
}
inline const std::string output_form(const std::string token)
{
  if("\n" == token)
    return std::string("\\n");
  else
    return token;
}
template<class T>
const T& map_value(const std::vector<T>& map,
                   const typename std::vector<T>::size_type key)
{
  return map[key];
}
// return mapped value
template<class T, class Y>
const T& map_value(const std::map<Y,T>& map,
                   const Y& key)
{
  return (*map.find(key)).second;
}
// put mapped value in value and return true if found, otherwise return false
template<class T, class Y>
bool map_value(const std::map<Y,T>& map,
               const Y& key,
               T& value)
{
  const auto it = map.find(key);
  if(it != map.end())
  {
    value = (*it).second;
    return true;
  }
  else
    return false;
}

/* Ambrosia dependent functions (use one or more of libAmbrosia's functions/classes)
 *******************************/
// replaces directory seperators ("/" or "\") with libambrosia::directory_seperator
const std::string replace_directory_seperators(const std::string& original);
// splits off everything before last forward slash
const string_pair split_preceding_directory(const std::string& path);
// remove Byte Order Mark from stream
void skip_BOM(std::istream& stream,
              const std::string& filename = "");

// file_type conversions
inline file_type get_general_type(const file_type type)
{
  switch(type)
  {
    case file_type::source:
    case file_type::source_c:
    case file_type::source_cxx:
    case file_type::source_java:
    case file_type::source_fortran:
      return file_type::source;
    case file_type::header:
    case file_type::header_c:
    case file_type::header_cxx:
      return file_type::header;
    default:
      return type;
  }
}
inline file_type detect_type(const file_type general_type, const std::string& filename)
{
  switch(general_type)
  {
    case file_type::source:
    {
      const std::string extension(get_extension(filename));
      if("c" == extension || "C" == extension)
        return file_type::source_c;
      else if("cpp" == extension || "cxx" == extension || "c++" == extension || "cc" == extension)
        return file_type::source_cxx;
      else if("java" == extension)
        return file_type::source_java;
      else if("f" == extension || "f90" == extension)
        return file_type::source_fortran;
    }
    default: // is actually already a specific type
      return general_type;
  }
}

// Dependency resolving algorithm
void dependency_resolve(target_vector& unsorted,
                        target_vector::iterator node,
                        target_vector& resolved,
                        target_vector& unresolved);
// Dependency resolving sort
void dependency_sort(target_vector& unsorted);
// Dependency resolving sort that filters out targets not present in s_ambrosia_config::targets_config
void filter_dependency_sort(target_vector& unsorted);

libambrosia_namespace_end

#endif // AMBROSIA_ALGORITHM_H
