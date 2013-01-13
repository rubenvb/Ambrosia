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
 * Ambrosia/external_dependency.h
 * Class representing an external dependency containing:
 *  - search directories
 *  - library files
 *  - ...
 * that are passed on the commandline so they can be automatically added to build commands when the dependent target is built.
 * This class is required (i.e. a simple pointer to another target is not sufficient) because external dependencies are handled
 *  quite differently: external dependencies can posess multiple library names for example, and their type is of no importance.
 *
 **/

#ifndef AMBROSIA_TARGET_EXTERNAL_DEPENDENCY_H
#define AMBROSIA_TARGET_EXTERNAL_DEPENDENCY_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/Target/target.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <memory>
#include <string>
#include <tuple>
#include <vector>

libambrosia_namespace_begin

struct external_dependency : public target
{
public:
  external_dependency(const std::string& name,
                      const std::string& include_directory,
                      const std::string& lib_directory,
                      const string_set& libraries,
                      const bool optional = false);
  external_dependency(const std::string& name,
                      const bool optional = false);

  bool operator<(const external_dependency& rhs) const
  { return name < rhs.name; }
  bool operator==(const external_dependency& rhs) const
  { return name == rhs.name; }

  const std::string name;
  std::string include_directory;
  std::string lib_directory;
  string_set libraries;
  bool optional;
};

libambrosia_namespace_end

namespace std
{
  template <>
  struct hash<libambrosia::external_dependency>
  {
    std::size_t operator()(const libambrosia::external_dependency& dep) const
    { return hash<string>()(dep.name); }
  };
}

#endif // AMBROSIA_TARGET_EXTERNAL_DEPENDENCY_H
