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
 * Ambrosia/Target/target.h
 * Abstract target base class.
 *
 **/

#ifndef AMBROSIA_TARGET_TARGET_H
#define AMBROSIA_TARGET_TARGET_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iterator>
#include <string>
#include <utility>

namespace ambrosia
{
namespace lib
{

class target
{
public:
  target(const std::string& name,
         const target_type type);

  bool operator<(const target& rhs) const
  { return (name < rhs.name) ? true : (type < rhs.type); }
  bool operator==(const target& rhs) const
  { return name == rhs.name && type == rhs.type; }

  // Build commands
  virtual void generate_commands()
  {   }
  virtual void dump_commands() const
  {   }
  virtual void execute_build_commands() const
  {   }

  // data
  std::string name;
  target_type type;

  virtual const std::string& source_directory() const = 0;
  std::map<file_type, build_element_set> files;
  std::map<file_type, string_set> directories;
  string_set libraries;

  command_vector parallel_commands;
  platform::command link_command;
};

} // namespace lib

} // namespace ambrosia

namespace std
{
  template <>
  struct hash<libambrosia::target>
  {
    std::size_t operator()(const libambrosia::target& dep) const
    { return hash<string>()(dep.name); }
  };
}

#endif // AMBROSIA_TARGET_TARGET_H
