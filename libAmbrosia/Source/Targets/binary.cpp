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
#include "Ambrosia/Error/command_error.h"
#include "Ambrosia/generator.h"

// C++ includes
#include <iterator>
  using std::insert_iterator;
#include <string>
  using std::string;

libambrosia_namespace_begin

binary::binary(const string& name,
               const ::libambrosia::configuration& configuration,
               const target_type type,
               const dependency_set& dependencies)
: target(name, configuration, type, dependencies)
{   }

void binary::generate_commands()
{
  debug(debug::command_gen) << "binary::generate_commands::Generating commands for binary: " << name << ".\n";

  string_set library_directories;
  string_vector libraries; // libraries to link to the final object file

  // Generate compilation commands per source file
  for(auto type_it = std::begin(files); type_it != std::end(files); ++type_it)
  {
    debug(debug::command_gen) << "binary::generate_commands::Generating commands for " << type_it->second.size() << " " << file_type_map_inverse.at(type_it->first) << " files.\n";
    string_set header_directories;
    // add the project's header dirs
    for(auto&& directory : directories.at(file_type::header))
    {
      header_directories.insert(configuration.source_directory / directory);
    }
    debug(debug::command_gen) << "binary::generate_commands::Current target direct dependencies: " << dependencies.size() << "\n";
    for(auto&& dependency :dependencies)
    {
      debug(debug::command_gen) << "binary::generate_commands::Including dependency \'" << dependency.name << "\'\'s header directories.\n";
      const string& source_directory = dependency.target->configuration.source_directory;
      const string_set& dependency_header_directories = dependency.target->directories.at(file_type::header);
      for(auto&& header_directory : dependency_header_directories)
      {
        debug(debug::command_gen) << "binary::generate_commands::Including directory: \'" << header_directory << "\'' with source directory " << source_directory << "\n";
        header_directories.insert(source_directory / header_directory);
      }
      if(dependency.type == target_type::library)
      {
        debug(debug::command_gen) << "binary::generate_commands::Including library " << dependency.name << " in (dynamic) linker command.\n"
                                      "\twith library search directory: " << dependency.target->configuration.build_directory << " and library name " << dependency.target->configuration.build_directory << ".\n";
        library_directories.insert(dependency.target->configuration.build_directory);
        libraries.push_back(dependency.target->name);
        for(auto&& library : dependency.target->libraries)
        {
          libraries.push_back(library);
        }
      }
    }
    debug(debug::command_gen) << "binary::generate_commands::Creating command generator for the " << vendor_map_inverse.at(configuration.target_toolchain) << " toolchain for " << os_map_inverse.at(configuration.target_os) << ".\n";
    generator command_generator(type_it->first, type_it->second, header_directories, configuration);
    command_generator.generate_object_filenames();
    command_generator.generate_parallel_commands(std::back_inserter(parallel_commands));
  }
  // Generate the final "link" command
  if(type == target_type::library)
  {
    //TODO: check static vs shared library
    link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_linker));
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_link_options));
    link_command.add_argument(configuration.build_directory / (toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_prefix) + name + toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_extension)));
  }
  else if(type == target_type::application)
  {
    if(contains(configuration.source_types, file_type::source_cxx))
    {
      debug(debug::command_gen) << "binary::generate_commands::Linking with C++ linker driver.\n";
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_cxx));
      if(contains(configuration.source_types, file_type::source_fortran))
        libraries.push_back(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::runtime_library_fortran));
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
    link_command.add_argument(configuration.build_directory / name  + os_options.at(configuration.target_os).at(os_option::executable_extension));
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
  debug(debug::command_gen) << "binary::generate_commands::Final command: " << link_command << "\n";
}

void binary::dump_commands() const
{
  for(auto&& command : parallel_commands)
  {
    debug(debug::command_gen) << "binary::dump_commands::Parallel command: " << command << "\n";
  }
  debug(debug::command_gen) << "binary::dump_commands::Final command: " << link_command << "\n";
}

void binary::execute_build_commands() const
{
  debug(debug::command_exec) << "binary::execute_build_commands::Building binary: " << name << ".\n";
  debug(debug::command_exec) << "binary::execute_build_commands::Creating build directory: " << configuration.build_directory << "\n";
  platform::create_directory_recursive(configuration.build_directory);

  for(auto&& command : parallel_commands)
  {
    string stdout_output;
    string stderr_output;
    int exit_code = execute_command(command, stdout_output, stderr_output);
    if(exit_code != 0)
    {
      debug(debug::command_exec) << "binary::execute_commands::Command returned failure.\n";
      throw command_error(stderr_output, command);
    }

    debug(debug::command_exec) << "binary::execute_build_commands::Command execution succesful:\n"
                                  "\tcommand was: " << command << "\n"
                                  "\tstdout: " << stdout_output << "\n"
                                  "\tstderr: " << stderr_output << "\n";
  }
  string stdout_output;
  string stderr_output;
  execute_command(link_command, stdout_output, stderr_output);
  debug(debug::command_exec) << "binary::execute_build_commands::Link command execution succesful!\n"
                                "\tcommand was: " << link_command << "\n"
                                "\tstdout: " << stdout_output << "\n"
                                "\tstderr: " << stderr_output << "\n";
}

libambrosia_namespace_end
