/**
  * Ambrosia/typedefs.h
  * Handy typedefs used throughout Ambrosia code as shorthand notation.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// Global includes
#include "global.h"

// libAmbrosia includes
#include "enums.h"

// C++ includes
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

ambrosia_namespace_begin

// STL typedefs
typedef std::pair<std::string, bool> pair_string_bool;
typedef std::pair<std::string, std::set<std::string> > pair_string_string_set;
typedef std::map<std::string, std::set<std::string> > map_string_set_string;
typedef std::vector<std::string> string_vector;
typedef std::set<std::string> string_set;
typedef std::map<std::string, std::string> string_map;
typedef std::pair<std::string, time_t> file;
typedef std::map<std::string, time_t> file_set;

ambrosia_namespace_end

libambrosia_namespace_begin

// libAmbrosia typedefs
class target;
typedef std::unique_ptr<target> target_ptr;
typedef std::vector<target_ptr> target_list;
typedef std::set<std::pair<target_type, std::string> > dependency_list;
typedef std::map<file_type, string_set> directory_map;
typedef std::map<file_type, std::map<std::string, file_set> > map_file_type_map_string_file_set;

libambrosia_namespace_end

#endif // TYPEDEFS_H
