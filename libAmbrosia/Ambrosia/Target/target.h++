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
#include "Ambrosia/global.h++"

// libAmbrosia includes
#include "Ambrosia/configuration.h++"
#include "Ambrosia/platform.h++"
#include "Ambrosia/typedefs.h++"

// C++ includes
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>

namespace ambrosia
{
namespace lib
{

class target
{
public:
  target(const std::string& name,
         const target_type type,
         const std::unordered_map<file_type, file_set>& files = std::unordered_map<file_type, file_set>(),
         const std::map<file_type, string_set>& directories = std::map<file_type, string_set>() );

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
  virtual void gather_dependency_information(string_set& header_directories,
                                             string_set& library_directories,
                                             string_vector& libraries);
  std::unordered_map<file_type, file_set> files;
  std::map<file_type, string_set> directories;
  string_set libraries;

  command_vector parallel_commands;
  command_vector final_commands;
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
