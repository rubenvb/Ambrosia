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
 * Ambrosia/Targets/app.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Targets/global.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/build_config.h"

libambrosia_namespace_begin

global::global(const build_config& configuration,
               const std::string& subdirectory)
: target(configuration)
{
  this->configuration.source_directory = full_directory_name(configuration.source_directory, subdirectory);
}

libambrosia_namespace_end
