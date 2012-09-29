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
 * Ambrosia/nectar.h
 * Functions to facilitate working with *.nectar.txt files
 *
 **/

#ifndef AMBROSIA_NECTAR_H
#define AMBROSIA_NECTAR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iterator>
#include <memory>
#include <set>
#include <string>

libambrosia_namespace_begin

// forward declarations
class configuration;
class target;

// locates a unique *.nectar.txt file in directory. More than one emits an error.
const std::string find_project_file(const std::string& directory);

// converts all project file content to internal representation, adding to the container where "it" outputs to
void drink_nectar(project& project);
// uses s_configuration to apply the requested build configuration
void apply_configuration(target_ptr_vector& targets);

libambrosia_namespace_end

#endif // AMBROSIA_NECTAR_H
