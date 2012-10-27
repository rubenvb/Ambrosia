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
 * Ambrosia/Targets/project.h
 * Project target used as top-level target and for "sub" targets.
 *
 **/

#ifndef AMBROSIA_TARGETS_PROJECT_H
#define AMBROSIA_TARGETS_PROJECT_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/file_cache.h"
#include "Ambrosia/Targets/target.h"

libambrosia_namespace_begin

class project : public target
{
public:
  project(const ::libambrosia::configuration& configuration);
  project(const std::string& name,
          const ::libambrosia::configuration& configuration,
          const dependency_set& dependencies = dependency_set());

  void generate_commands();

  target_ptr_vector targets;

  ::libambrosia::file_cache file_cache;

private:
  const std::string filename;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_PROJECT_H
