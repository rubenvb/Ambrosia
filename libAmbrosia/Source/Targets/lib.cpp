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

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

lib::lib(const string& name,
         const ::libambrosia::configuration& configuration,
         const dependency_set& dependencies)
: target(configuration, name, dependencies)
{   }

libambrosia_namespace_end
