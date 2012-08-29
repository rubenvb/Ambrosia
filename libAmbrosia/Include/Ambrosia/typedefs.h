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
 * Ambrosia/typedefs.h
 * Handy typedefs used throughout Ambrosia code as shorthand notation.
 *
 **/

#ifndef AMBROSIA_TYPEDEFS_H
#define AMBROSIA_TYPEDEFS_H

// Global includes
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/build_element.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/file.h"

// C++ includes
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

/*
 * STL typedefs
 ***************/

ambrosia_namespace_begin

// Homogeneous typedefs
typedef std::pair<std::string, std::string> string_pair;
typedef std::map<std::string, std::string> string_map;
typedef std::vector<std::string> string_vector;
typedef std::unordered_set<std::string> string_set;

// Heterogeneous typedefs
typedef std::pair<std::string, bool> pair_string_bool;
typedef std::pair<std::string, string_set> pair_string_string_set;
typedef std::map<std::string, string_set> map_string_set_string;
typedef std::map<std::string, string_vector> map_string_string_vector;

ambrosia_namespace_end

/*
 * libAmbrosia typedefs
 ***********************/
libambrosia_namespace_begin

// Forward declarations
class project;
class target;

// homogeneous typedefs
typedef std::unordered_set<file> file_set;
typedef std::unordered_set<build_element> build_element_set;
typedef std::unique_ptr<project> project_ptr;
typedef std::vector<project_ptr> project_vector;
typedef std::unique_ptr<target> target_ptr;
typedef std::vector<target_ptr> target_vector;

// heterogeneous typedefs
typedef std::set<std::tuple<target_type, std::string, target*>> dependency_set;
typedef std::map<generator_string, std::string> generator_map;
typedef std::map<file_type, std::map<toolchain, generator_map>> command_map;
typedef std::map<file_type, build_element_set> map_file_type_build_element_set;
typedef std::map<file_type, string_set> map_file_type_string_set;
typedef std::map<std::string, file_set> map_string_file_set;

libambrosia_namespace_end

#endif // AMBROSIA_TYPEDEFS_H
