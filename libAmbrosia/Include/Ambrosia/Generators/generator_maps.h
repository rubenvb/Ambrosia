/**
  * Ambrosia/Generators/generator_maps.h
  * Command building blocks for all toolchains/languages.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef GENERATOR_MAPS_H
#define GENERATOR_MAPS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// C-style languages
extern const generator_map cgenerator_map;
extern const generator_map cxxgenerator_map;
extern const generator_map objcgenerator_map;
extern const generator_map objcxxgenerator_map;

// Fortran, java, etc..
// ...

libambrosia_namespace_end

#endif // GENERATOR_MAPS_H
