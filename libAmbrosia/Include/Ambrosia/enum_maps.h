/**
  * Ambrosia/enum_maps.h
  * Ambrosia's handy enums mapped to their string for input parsing and output.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "enums.h"

// C++ includes
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

libambrosia_namespace_end

#endif // ENUM_MAPS_H