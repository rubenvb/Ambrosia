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
 * Ambrosia/Targets/binary.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Targets/binary.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/generator.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

binary::binary(const string& name,
               const ::libambrosia::configuration& configuration,
               const target_type type,
               const dependency_set& dependencies)
: target(name, configuration, type, dependencies),
  parallel_commands(),
  link_command()
{   }

void binary::generate_commands()
{
  debug(debug::command_gen) << "binary::generate_commands::Generating commands for binary: " << name << ".\n";

  // Generate compilation commands per source file
  for(auto type_it = std::begin(files); type_it != std::end(files); ++type_it)
  {
    debug(debug::command_gen) << "binary::generate_commands::Generating commands for " << type_it->second.size() << " " << file_type_map_inverse.at(type_it->first) << " files.\n";
    generator command_generator(type_it->first, type_it->second, source_directories.at(file_type::header), configuration);
    command_generator.generate_object_filenames();
    command_generator.generate_parallel_commands(std::back_inserter(parallel_commands));
  }
  // Generate the final "link" command
  if(type == target_type::library)
  {
    //TODO: check static vs shared library
    link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_linker));
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_link_options));
  }
  else if(type == target_type::application)
  {
    link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker));
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::output_object));
    link_command.add_argument(full_directory_name(configuration.build_directory, configuration.name)
                                                  +os_options.at(configuration.target_os).at(os_option::executable_extension));
  }
  // add all object files with full path (look into "response files" or "object scripts" to circumvent commandline length limits)
  for(auto type_it = std::begin(files); type_it != std::end(files); ++type_it)
  {
    build_element_set& bes = type_it->second;
    for(auto bes_it = std::begin(bes); bes_it != std::end(bes); ++bes_it)
    {
      link_command.add_argument(bes_it->object_file.name);
    }
  }
  debug(debug::command_gen) << "binary::generate_commands::Final command: " << link_command << "\n";
}

libambrosia_namespace_end
