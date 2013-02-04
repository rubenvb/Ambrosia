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
 * Ambrosia/link_generator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/link_generator.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/configuration.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/platform.h"

namespace ambrosia
{
namespace lib
{

/*link_generator::link_generator(const class configuration& configuration,
                               const string_vector& libraries,
                               const std::map<file_type, build_element_set>& files)
: configuration(configuration),
  libraries(libraries),
  files(files)
{   }

const command_vector link_generator::generate_final_commands() const
{
  command_vector commands;
  platform::command link_command;
  if(type == target_type::library)
  {
    //TODO: check static vs shared library
    link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_linker));
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_link_options));
    link_command.add_argument(configuration.build_directory / (toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_prefix) + configuration.name + toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_extension)));
  }
  else if(type == target_type::application)
  {
    if(contains(configuration.source_types, file_type::source_cxx))
    {
      debug(debug::command_gen) << "binary::generate_commands::Linking with C++ linker driver.\n";
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_cxx));
      if(contains(configuration.source_types, file_type::source_fortran))
        libraries.insert(std::begin(libraries), toolchain_options.at(configuration.target_toolchain).at(toolchain_option::runtime_library_fortran));
    }
    else if(contains(configuration.source_types, file_type::source_fortran))
    {
      debug(debug::command_gen) << "binary::generate_commands::Linking with Fortran linker driver.\n";
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_fortran));
    }
    else
    {
      debug(debug::command_gen) << "binary::generate_commands::Linking with C linker driver.\n";
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_c));
    }

    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::output_object));
    link_command.add_argument(configuration.build_directory / configuration.name  + os_options.at(configuration.target_os).at(os_option::executable_extension));
  }
  for(auto type_it = std::begin(files); type_it != std::end(files); ++type_it)
  {
    for(auto&& build_element : type_it->second)
    {
      link_command.add_argument(build_element.object_file.name);
    }
  }
  // add all library search directories
  for(auto&& library_directory : library_directories)
  {
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::link_search_directory) + library_directory);
  }
  // add all libraries to link
  for(auto&& library : libraries)
  {
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::link_library) + library);
  }
  commands.push_back(link_command);
  return commands;
}
*/
} // namespace lib

} // namespace ambrosia
