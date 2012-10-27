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
 * Ambrosia/Targets/lib.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Targets/lib.h"

// Ambrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/Generators/generator.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

lib::lib(const string& name,
         const ::libambrosia::configuration& configuration,
         const dependency_set& dependencies)
: target(configuration, name, dependencies)
{   }

void lib::generate_commands()
{
  for(auto type_it = std::begin(files); type_it != std::end(files); ++type_it)
  {
    debug(debug::command_gen) << "lib::generate_commands::Generating commands for " << type_it->second.size() << " " << file_type_map_inverse.at(type_it->first) << " files.\n";
    generator command_generator(type_it->first, type_it->second, source_directories.at(file_type::header), configuration);
  }
  throw error("lib::generate_commands not completely implemented yet.");
}

libambrosia_namespace_end
