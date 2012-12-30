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
 * Ambrosia/Targets/binary.h
 * Binary targets, with a library or app as output.
 *
 **/

#ifndef BINARY_H
#define BINARY_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Targets/target.h"

libambrosia_namespace_begin

class binary : public target
{
public:
  // project local target
  binary(const std::string& name,
         const ::libambrosia::configuration& configuration,
         const target_type type,
         const dependency_set& dependencies = dependency_set());

  void generate_commands();
  void dump_commands() const;
  void execute_build_commands() const;
};

libambrosia_namespace_end

#endif // BINARY_H
