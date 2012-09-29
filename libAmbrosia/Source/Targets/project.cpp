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

// libAmbrosia includes

libambrosia_namespace_begin

project::project(const ::libambrosia::configuration& configuration)
: target(configuration)
{   }
project::project(const std::string& name,
                 const ::libambrosia::configuration& configuration,
                 const dependency_set& dependencies)
: target(configuration, name, dependencies),
  file_cache(::libambrosia::file_cache())
{   }

libambrosia_namespace_end
