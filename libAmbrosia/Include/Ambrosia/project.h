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
 * Ambrosia/project.h
 * Convenienve class containing everything Ambrosia needs and does.
 *  Everything that uses Ambrosia (plugins, etc...) should go through
 *  here, if possible.
 *
 **/

#ifndef AMBROSIA_PROJECT_H
#define AMBROSIA_PROJECT_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/build_element.h"
#include "Ambrosia/status.h"
//#include "Ambrosia/target.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class file_cache;
/*
class project : public target
{
public:
  project(const std::string& name, ambrosia_config& configuration);

  // Determine projects to be built and apply build configuration.
  void apply_target_configuration();

  // Output the build commands
  void dump_commands() const;

  // Run the build commands.
  void execute_build_commands();

  target_ptr_vector m_targets;

private:
  map_string_string_vector m_commands; // commands per target, easier for debugging, info and fancy output.
};*/

libambrosia_namespace_end

#endif // AMBROSIA_PROJECT_H
