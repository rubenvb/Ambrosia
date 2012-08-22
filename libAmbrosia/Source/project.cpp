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
 * Ambrosia/project.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/project.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/build_element.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/Generators/compile_and_link_generator.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
#include <memory>
  using std::unique_ptr;
#include <string>
  using std::string;
#include <fstream>
  using std::ifstream;

libambrosia_namespace_begin

ambrosia_config* project::configuration = NULL;

project::project(ambrosia_config& ambrosia_config,
                 file_cache& file_cache)
: m_targets(),
  m_commands(),
  m_file_cache(file_cache)
{
  configuration = &ambrosia_config;
}
project::project(file_cache& file_cache)
: m_targets(),
  m_commands(),
  m_file_cache(file_cache)
{   }

// Filter target_list and sort the targets that need to be built.
void project::apply_target_configuration()
{
  const map_string_set_string target_config = configuration->target_config_options();

  for(auto&& it = std::begin(target_config); it != std::end(target_config); ++it)
  {

  }
  throw error("project::apply_target_configuration::Not yet implemented.");
}

// Configuration is applied, order is fixed. Generate output names
/*const string_vector project::determine_output_files()
{
    for( auto it = m_targets.begin(); it != m_targets.end(); ++it )
    {

    }
}*/

void project::generate_commands()
{
  // assume targets are in the correct dependent order
  for(auto&& target_it = std::begin(m_targets); target_it != std::end(m_targets); ++target_it)
  {
    target& current = **target_it;
    if(current.m_type == target_type::global)
    {
      debug(debug::command_gen) << "project::generate_commands::Skipping generation of build commands for target: " << current.name << ".\n";
      continue;
    }
    debug(debug::command_gen) << "project::generate_commands::Generating build commands for target: " << current.name << "\n"
                              << "\tfor output in this directory: " << current.m_build_config.m_build_directory << "\n"
                              << "\tfor the following types of source files:\n"
                              << "\t" << current.m_build_config.m_source_types << "\n";

    // verify the build config is OK to use
    current.verify_configuration();

    for(auto&& type_it = std::begin(current.m_build_config.m_source_types); type_it != std::end(current.m_build_config.m_source_types); ++type_it)
    {
      const auto& type = *type_it;
      debug(debug::command_gen) << "project::generate_commands::Generating commands for " << current.files(type).size() << " "
                                << file_type_map_inverse.at(type) << " files.\n";
      //TODO generalize to a "get_generator function when there are languages supported that need a different style of processing
      unique_ptr<generator> generator(new compile_and_link_generator(type, current));

      generator->generate_object_filenames();
      const string_vector commands = generator->generate_parallel_commands();

      //TODO: debug output of commands, or storage in a per-target list for nice output.

      //TODO: fix ugly function call below
      m_commands[current.name].insert(std::end(m_commands[current.name]), std::begin(commands), std::end(commands));
    }
  }
  throw error("generate_commands is not completely implemented yet.");
}

void project::execute_build_commands()
{
  throw error("execute_build_commands is not implemented yet.");
}

libambrosia_namespace_end
