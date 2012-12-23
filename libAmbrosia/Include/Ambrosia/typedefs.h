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
#include "Ambrosia/dependency.h"
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
struct external_dependency;
namespace platform
{
struct command;
}

// homogeneous typedefs
typedef std::unordered_set<file> file_set;
typedef std::unordered_set<build_element> build_element_set;
typedef std::set<dependency> dependency_set;
typedef std::unique_ptr<target> target_ptr;
typedef std::vector<std::unique_ptr<target>> target_ptr_vector;
typedef std::vector<platform::command> command_vector;

// heterogeneous typedefs
typedef std::map<file_type, build_element_set> map_file_type_build_element_set;
typedef std::map<file_type, string_set> map_file_type_string_set;
typedef std::map<std::string, file_set> map_string_file_set;
typedef std::map<std::string, external_dependency> map_string_external_dependency;

typedef std::map<toolchain_option, std::string> toolchain_option_map;
typedef std::map<language_option, std::string> language_option_map;
typedef std::map<os_option, std::string> os_option_map;

libambrosia_namespace_end

#endif // AMBROSIA_TYPEDEFS_H
