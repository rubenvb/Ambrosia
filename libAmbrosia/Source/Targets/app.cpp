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
#include "Ambrosia/Targets/app.h"

// Ambrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

app::app(const string& name,
         const ::libambrosia::configuration& configuration,
         const dependency_set& dependencies)
: target(configuration, name, dependencies)
{   }

void app::generate_commands()
{
  throw error("app::generate_commands not implemented yet.");
}


libambrosia_namespace_end
