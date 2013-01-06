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
 * Ambrosia/enum_maps.h
 * Ambrosia's handy enums mapped to their string for input parsing and output.
 *
 **/

#ifndef AMBROSIA_ENUM_MAPS_H
#define AMBROSIA_ENUM_MAPS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <functional>
#include <map>
#include <string>

libambrosia_namespace_begin

extern const std::map<std::string, architecture> architecture_map;
extern const std::map<architecture, std::string> architecture_map_inverse;
extern const std::map<std::string, os> os_map;
extern const std::map<os, std::string> os_map_inverse;
extern const std::map<std::string, toolchain> vendor_map;
extern const std::map<toolchain, std::string> vendor_map_inverse;
extern const std::map<std::string, toolchain> toolchain_map;
extern const std::map<toolchain, std::string> toolchain_map_inverse;

extern const std::map<std::string, target_type> target_type_map;
extern const std::map<target_type, std::string> target_type_map_inverse;
extern const std::map<std::string, file_type> file_type_map;
extern const std::map<file_type, std::string> file_type_map_inverse;
extern const std::map<std::string, file_type> directory_type_map;
extern const std::map<file_type, std::string> directory_type_map_inverse;

extern const std::map<std::string, conditional_operator> conditional_operator_map;
extern const std::map<conditional_operator, std::string> conditional_operator_map_inverse;
extern const std::map<conditional_operator, std::function<bool(bool,bool)>> conditional_function_map;

extern const std::map<toolchain, toolchain_option_map> toolchain_options;
extern const std::map<toolchain, std::map<file_type, language_option_map>> language_options;
extern const std::map<os, os_option_map> os_options;

libambrosia_namespace_end

#endif // AMBROSIA_ENUM_MAPS_H
