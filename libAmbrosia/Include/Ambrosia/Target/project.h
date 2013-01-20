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
 * Ambrosia/Target/project.h
 * Project target used as top-level target and for "sub" targets.
 *
 **/

#ifndef AMBROSIA_TARGET_PROJECT_H
#define AMBROSIA_TARGET_PROJECT_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/file_cache.h"
#include "Ambrosia/Target/target.h"

libambrosia_namespace_begin

class project : public target
{
public:
  project(const ::libambrosia::configuration& configuration);
  project(const std::string& name,
          const ::libambrosia::configuration& configuration,
          const dependency_map& dependencies = dependency_map());

  void generate_commands(); // generate build commands for all targets
  void dump_commands() const; // dump commands of all targets
  void execute_build_commands() const; // builds targets in parallel if possible

  const std::string& source_directory() const
  { return configuration.source_directory; }

  lib::configuration configuration;

  target_ptr_vector targets;

  ::libambrosia::file_cache file_cache;

private:
  const std::string filename;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGET_PROJECT_H
