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
 * Ambrosia/Targets/target.cpp
 * Class implementation
 *
 **/

// Class include
#include "Ambrosia/Target/target.h"

// Ambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/nectar_error.h"
#include "Ambrosia/file_cache.h"

// C++ includes
#include <cstddef>
  using std::size_t;
#include <string>
  using std::string;

libambrosia_namespace_begin

target::target(const string& name,
               const target_type type,
               const dependency_map& dependencies)
: name(name),
  type(type),
  dependencies(dependencies),
  files(),
  directories(),
  parallel_commands(),
  link_command()
{   }

libambrosia_namespace_end
