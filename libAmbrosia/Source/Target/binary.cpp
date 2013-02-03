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
#include "Ambrosia/Target/binary.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/command_error.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/generator.h"

// C++ includes
#include <iterator>
  using std::insert_iterator;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

binary::binary(const string& name,
               const ::libambrosia::configuration& configuration,
               const target_type type,
               const dependency_map& dependencies)
: target(name, type),
  configuration(configuration),
  dependencies(dependencies)
{
  this->configuration.build_directory = configuration.build_directory / name;
  // Set default output directories:
  directories[file_type::library].insert(this->configuration.build_directory);
  directories[file_type::executable].insert(this->configuration.build_directory);
}

void binary::add_source_file(const file_type general_type,
                             const string& filename,
                             file_cache& file_cache,
                             const string& /*nectar_file*/,
                             const size_t /*line_number*/)
{
  // add source file type to list
  // search specific file_type directories
  const file_type specific_type = detect_type(general_type, filename);
  string_set& specific_directories = directories[specific_type];
  string_set& general_directories = directories[general_type];
  specific_directories.insert(std::begin(general_directories), std::end(general_directories));
  debug(debug::target) << "binary::add_source_file::Finding " << file_type_map_inverse.at(specific_type) << " files matching " << filename << " in:\n"
                      << specific_directories << "\n";
  file_cache.find_source_files(filename, configuration.source_directory, specific_directories, files[specific_type]);
  configuration.source_types.insert(specific_type);
  //if(general_type != specific_type)
  //  file_cache.find_source_files(filename, configuration.source_directory, general_directories, files[general_type]);
}
bool binary::add_source_directory(const file_type type,
                                  const string& directory,
                                  file_cache& file_cache)
{
  debug(debug::target) << "binary::add_source_directory::Adding directory " << directory << " of type " << file_type_map_inverse.at(type) << ".\n";
  if(!file_cache.add_source_directory(configuration.source_directory / directory))
    return false;
  if(!directories[type].insert(directory).second)
    debug(debug::target) << "binary::add_source_directory::Directory " << directory << " already present.\n";

  const file_type general_type = get_general_type(type);
  if(type != general_type)
  {
    debug(debug::target) << "binary::add_source_directory::Adding directory " << directory << " of general type " << file_type_map_inverse.at(type) << ".\n";
    if(!directories[general_type].insert(directory).second)
      debug(debug::target) << "binary::add_source_directory::Directory " << directory << " already present.\n";
  }

  return true;
}

void binary::add_library(const string& library,
                         const string& /*nectar_file*/,
                         const size_t /*line_number*/)
{
  libraries.insert(library);
}

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
    debug(debug::command_gen) << "binary::generate_commands::Current target direct library dependencies: " << dependencies[target_type::library].size() << "\n";
    for(auto&& dependency : dependencies.at(target_type::library))
    {
      debug(debug::command_gen) << "binary::generate_commands::Including dependency " << dependency->name << "\'s header directories.\n";
      const string& source_directory = dependency->source_directory();
      const string_set& dependency_header_directories = dependency->directories.at(file_type::header);
      if(dependency->type == target_type::external)
      {
        debug(debug::command_gen) << "binary::generate_commands::Include dependency " << dependency->name << " header directories:\n";
        for(auto&& header_directory : dependency_header_directories)
        {
          debug(debug::command_gen) << "\t" << header_directory << "\n";
          header_directories.insert(header_directory);
        }
      }
      else
      {
        for(auto&& header_directory : dependency_header_directories)
        {
          debug(debug::command_gen) << "binary::generate_commands::Including directory: " << header_directory << " with source directory " << source_directory << "\n";
          header_directories.insert(source_directory / header_directory);
        }
      }
      if(dependency->type == target_type::library)
      {
        debug(debug::command_gen) << "binary::generate_commands::Including library " << dependency->name << " in (dynamic) linker command.\n"
                                     "\twith library search directory: " << dependency->directories[file_type::library] << " and library names " << dependency->libraries << ".\n";
        library_directories.insert(std::begin(dependency->directories[file_type::library]), std::end(dependency->directories[file_type::library]));
        libraries.push_back(dependency->name);
        for(auto&& library : dependency->libraries)
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
    auto result = execute_command(command, stdout_output, stderr_output);
    if(!result.first)
    {
      debug(debug::command_exec) << "binary::execute_build_commands::failed to execute command: " << command << "\n";
      throw internal_error("Failed to execute build command.");
    }
    debug(debug::command_exec) << "binary::execute_commands::Command executed succesfully, returned: " << result.second << ".\n";
    if(result.second != 0)
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
  auto result = execute_command(link_command, stdout_output, stderr_output);
  if(!result.first)
  {
    debug(debug::command_exec) << "binary::execute_build_commands::failed to execute command: " << link_command << "\n";
    throw internal_error("Failed to execute link command.");
  }
  if(result.second != 0)
  {
    debug(debug::command_exec) << "binary::execute_commands::Command returned failure.\n" << stderr_output << "\n";
    throw command_error(stderr_output, link_command);
  }
  debug(debug::command_exec) << "binary::execute_build_commands::Link command execution succesful!\n"
                                "\tcommand was: " << link_command << "\n"
                                "\tstdout: " << stdout_output << "\n"
                                "\tstderr: " << stderr_output << "\n";
}

} // namespace lib

} // namespace ambrosia
