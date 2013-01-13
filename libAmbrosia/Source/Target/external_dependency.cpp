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
 * Ambrosia/Targets/external_dependency.cpp
 * Class implementation
 *
 **/

// Class include
#include "Ambrosia/Target/external_dependency.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/Target/target.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

external_dependency::external_dependency(const std::string& name,
                                         const std::string& include_directory,
                                         const std::string& lib_directory,
                                         const string_set& libraries,
                                         const bool optional)
: target(name, target_type::external),
  name(name),
  include_directory(include_directory),
  lib_directory(lib_directory),
  libraries(libraries),
  optional(optional)
{   }

external_dependency::external_dependency(const std::string& name,
                                         const bool optional)
: target(name, target_type::external),
  name(name),
  include_directory(),
  lib_directory(),
  libraries(),
  optional(optional)
{   }

libambrosia_namespace_end
