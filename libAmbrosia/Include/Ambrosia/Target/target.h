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

libambrosia_namespace_begin

class target
{
public:
  target(const std::string& name,
         const target_type type);

  // Build commands
  virtual void generate_commands()
  {   }
  virtual void dump_commands() const
  {   }
  virtual void execute_build_commands() const
  {   }

  std::string name;
  target_type type;

  std::map<file_type, build_element_set> files;
  std::map<file_type, string_set> directories;
  string_set libraries;

  command_vector parallel_commands;
  platform::command link_command;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGET_TARGET_H
