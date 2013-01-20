/**
 *
 * Project Ambrosia: Ambrosia commandline build tool
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
 * Ambrosia/dependency_paths.h
 * Struct containing
 *
 **/

#ifndef AMBROSIA_DEPENDENCY_PATHS_H
#define AMBROSIA_DEPENDENCY_PATHS_H

// Global include
#include "Ambrosia/global.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

struct dependency_paths
{
  dependency_paths(const std::string& name) : name(name), include(), lib(), bin(), pkgconfig() {}
  std::string name;
  // mutable to allow edits from std::[unordered_]set elements
  mutable std::string include;
  mutable std::string lib;
  mutable std::string bin;
  mutable std::string pkgconfig;

  bool operator<(const dependency_paths& rhs) const
  { return name < rhs.name; }
  bool operator==(const dependency_paths& rhs) const
  { return name == rhs.name; }
};

libambrosia_namespace_end

namespace std
{
  template <>
  struct hash<libambrosia::dependency_paths>
  {
    std::size_t operator()(const libambrosia::dependency_paths& dep) const
    { return hash<string>()(dep.name); }
  };
}

#endif // AMBROSIA_DEPENDENCY_PATHS_H
