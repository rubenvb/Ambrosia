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
 * Ambrosia/Targets/dependency.h
 * Class representing a "dep" target, which is a (collection of) target(s) that provide information such as:
 *  - search directories
 *  - output files
 *  - ...
 * that are known so they can be automatically added to build commands when the dependent target is built.
 *
 **/

#ifndef AMBROSIA_DEPENDENCY_H
#define AMBROSIA_DEPENDENCY_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"

// C++ includes
#include <string>
#include <vector>

libambrosia_namespace_begin

// Forward declarations
struct target;

struct dependency
{
public:
  dependency(const std::string& name,
             const target_type type,
             const libambrosia::target* target = nullptr,
             const bool optional = false)
  : name(name), type(type), target(target), optional(optional)
  {   }

  const std::string name;
  const target_type type;
  const libambrosia::target* target;
  const bool optional;
};

inline bool operator<(const dependency& dep1, const dependency& dep2)
{
  return dep1.name < dep2.name && dep1.type < dep2.type;
}

libambrosia_namespace_end

#endif // AMBROSIA_DEPENDENCY_H
