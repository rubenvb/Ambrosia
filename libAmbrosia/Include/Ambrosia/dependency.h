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
 * Ambrosia/dependency.h
 * Class representing an internal or external dependency target, which is a target that provides information such as:
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
#include <memory>
#include <string>
#include <tuple>
#include <vector>

libambrosia_namespace_begin

// Forward declarations
class target;

struct dependency
{
public:
  dependency(const std::string& name,
             const target_type type,
             ::libambrosia::target* = nullptr,
             const bool external = false,
             const bool optional = false);

  bool operator<(const dependency& rhs) const
  { return (name < rhs.name)?true:(type<rhs.type); }
  bool operator==(const dependency& rhs) const
  { return (name == rhs.name) && (type == rhs.type); }

  const std::string name;
  const target_type type; // do not use target_type::external here!
  ::libambrosia::target* target;
  const bool external;
  const bool optional;
};

libambrosia_namespace_end

namespace std
{
  template <>
  struct hash<libambrosia::dependency>
  {
    std::size_t operator()(const libambrosia::dependency& dep) const
    { return hash<string>()(dep.name) ^ hash<int>()(static_cast<int>(dep.type)); }
  };
}

#endif // AMBROSIA_DEPENDENCY_H
