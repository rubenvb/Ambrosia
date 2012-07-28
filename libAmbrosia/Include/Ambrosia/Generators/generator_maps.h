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
 * Ambrosia/Generators/generator_maps.h
 * Command building blocks for all toolchains/languages.
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

// map to map the maps according to file_type
extern const command_map the_huge_command_map;

libambrosia_namespace_end

#endif // GENERATOR_MAPS_H
