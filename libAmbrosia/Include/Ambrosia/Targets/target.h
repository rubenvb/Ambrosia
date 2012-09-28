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
 * Ambrosia/Targets/target.h
 * Abstract target base class.
 *
 **/

#ifndef AMBROSIA_TARGETS_TARGET_H
#define AMBROSIA_TARGETS_TARGET_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

// Forward declarations
class build_config;

class target
{
public:
  target(const build_config& configuration,
         const std::string& name = "",
         const dependency_set& dependencies = dependency_set());

  const std::string name;
  build_config configuration;
  const dependency_set dependencies;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_TARGET_H
