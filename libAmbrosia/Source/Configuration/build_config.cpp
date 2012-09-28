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
 * Ambrosia/Configuration/build_config.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Configuration/build_config.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

build_config::build_config(const string& subdirectory,
                           const string& target_name_suffix,
                           const config_base& config)
: config_base(config),
  source_types(),
  user_variables()
{
  source_directory = full_directory_name(source_directory, subdirectory);
  debug(debug::config) << "build_config::build_config::Build directory for " << source_directory << " was: \"" << build_directory << "\".\n";
  // check multiple 'sub' levels for correctness below:
  build_directory = full_directory_name(build_directory, subdirectory + target_name_suffix);
  debug(debug::config) << "build_config::build_config::Build directory for " << source_directory << " is now: \"" << build_directory << "\".\n";
}
build_config::build_config(const ambrosia_config& configuration)
: config_base(configuration),
  source_types(),
  user_variables()
{   }

libambrosia_namespace_end
