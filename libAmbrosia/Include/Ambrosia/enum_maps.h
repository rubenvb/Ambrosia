/**
  * Ambrosia/enum_maps.h
  * Ambrosia's handy enums mapped to their string for input parsing and output.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ENUM_MAPS_H
#define AMBROSIA_ENUM_MAPS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"

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
extern const std::map<std::string, environment> environment_map;
extern const std::map<environment, std::string> environment_map_inverse;

extern const std::map<std::string, target_type> target_type_map;
extern const std::map<target_type, std::string> target_type_map_inverse;
extern const std::map<std::string, file_type> file_type_map;
extern const std::map<file_type, std::string> file_type_map_inverse;
extern const std::map<std::string, file_type> directory_type_map;
extern const std::map<file_type, std::string> directory_type_map_inverse;

extern const std::map<std::string, conditional_operator> conditional_operator_map;
extern const std::map<conditional_operator, std::string> conditional_operator_map_inverse;
extern const std::map<conditional_operator, std::function<bool(bool,bool)> > conditional_function_map;

libambrosia_namespace_end

#endif // AMBROSIA_ENUM_MAPS_H
