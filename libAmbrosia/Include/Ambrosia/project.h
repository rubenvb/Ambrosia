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
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class file_cache;

class project
{
public:
  project(file_cache& file_cache);
  project(ambrosia_config& ambrosia_config,
          file_cache& file_cache);

  // Determine projects to be built and apply build configuration.
  void apply_target_configuration();

  // Determine the names and locations of output files to determine commands to be generated.
  // NEEDS REDESIGN AND SMARTNESS THAT IS NOW ABSENT
  //void determine_output_files();

  // Generate commands only for the files that need to be built.
  void generate_commands();

  // Run the build commands.
  void execute_build_commands();

  target_vector m_targets;

  static ambrosia_config* configuration;

private:
  string_vector m_commands;
  file_cache& m_file_cache;
};

libambrosia_namespace_end

#endif // AMBROSIA_PROJECT_H
