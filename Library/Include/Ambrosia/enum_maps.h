/**
  * enum_maps.h
  * Ambrosia's handy enums mapped to their string for input parsing and output.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"

// C++ includes
#include <map>
#include <string>

namespace ambrosia
{
    extern const std::map<std::string, os> os_map;
    extern const std::map<std::string, architecture> architecture_map;
    extern const std::map<std::string, toolchain> vendor_map;
    extern const std::map<std::string, toolchain> toolchain_map;
} // namespace ambrosia

#endif // ENUM_MAPS_H
