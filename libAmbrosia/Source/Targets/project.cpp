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
 * Ambrosia/Targets/project.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Targets/project.h"

// Ambrosia includes
#include "Ambrosia/debug.h"

// C++ includes
#include <algorithm>
#include <string>
  using std::string;

libambrosia_namespace_begin

project::project(const ::libambrosia::configuration& configuration)
: target("unknown", configuration, target_type::project),
  file_cache()
{   }
project::project(const string& name,
                 const ::libambrosia::configuration& configuration,
                 const dependency_set& dependencies)
: target(name, configuration, target_type::project, dependencies),
  file_cache()
{   }

void project::generate_commands()
{
  debug(debug::command_gen) << "project::generate_commands::Generating commands for project " << name << ".\n";
  std::for_each(std::begin(targets), std::end(targets),[](target_ptr& t) { t->generate_commands(); });
}


libambrosia_namespace_end
