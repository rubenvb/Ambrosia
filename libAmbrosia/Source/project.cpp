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
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/Generators/generator.h"
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
: m_commands(),
  m_file_cache(file_cache),
  m_targets()
{
  configuration = &ambrosia_config;
}
project::project(file_cache& file_cache)
: m_commands(),
  m_file_cache(file_cache),
  m_targets()
{   }

// read project file(s) and store the info and dependencies in target_list
void project::read_project_files()
{
  // open file
  const string& filename = configuration->project_file();
  auto stream_ptr = open_ifstream(filename);
  auto& stream = *stream_ptr;
  if(!stream )
    throw error("Unable to open *.nectar.txt file: " + filename);

  // read targets
  debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
  nectar_loader loader(filename, "", stream);

  loader.extract_nectar(m_targets);
}

// Filter target_list and sort the targets that need to be built.
void project::apply_target_configuration()
{
  const map_string_set_string target_config = configuration->target_config_options();

  for(auto it = target_config.begin(); it != target_config.end(); ++it)
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
  for(auto target_it = m_targets.begin(); target_it != m_targets.end(); ++target_it)
  {
    const target& current = **target_it;
    if(current.m_type == target_type::global)
    {
      debug(debug::command_gen) << "project::generate_commands::Skipping generation of build commands for target: " << current.name() << "\n";
      continue;
    }
    debug(debug::command_gen) << "project::generate_commands::Generating build commands for target: " << current.name() << "\n"
                              << "\tfor the following types of source files:\n"
                              << "\t" << current.m_build_config.m_source_types << "\n";

    for(auto type_it = current.m_build_config.m_source_types.begin(); type_it != current.m_build_config.m_source_types.end(); ++type_it)
    {
      const auto& type = *type_it;
      //const auto& source_files = current.source_files(type);
      unique_ptr<generator> generator = get_generator(type, current.m_build_config);
    }
  }
  throw error("generate_commands is not implemented yet.");
}

void project::execute_build_commands()
{
  throw error("execute_build_commands is not implemented yet.");
}

libambrosia_namespace_end
