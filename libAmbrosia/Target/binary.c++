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
 * Targets/binary.c++
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Target/binary.h++"

// libAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/Error/command_error.h++"
#include "Ambrosia/Error/internal_error.h++"
#include "Ambrosia/file_cache.h++"

// C++ includes
#include <ctime>
#include <iterator>
  using std::insert_iterator;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

binary::binary(const string& name,
               const target_type type,
               const std::string& build_type,
               const std::string& linkage,
               const ::libambrosia::configuration& configuration,
               const std::unordered_map<file_type, file_set>& files,
               const std::map<file_type, string_set>& directories,
               const dependency_map& dependencies)
: target(name, type, files, directories),
  configuration(configuration),
  dependencies(dependencies)
{
  this->configuration.name = name;
  this->configuration.config_strings.insert(build_type);
  this->configuration.config_strings.insert(linkage);
  this->configuration.build_directory = configuration.build_directory / name / linkage / build_type;
  // Set default output directories:
  this->directories[file_type::library].insert(this->configuration.build_directory);
  this->directories[file_type::executable].insert(this->configuration.build_directory);
}

void binary::generate_commands()
{
  debug(debug::command_gen, "binary::generate_commands::Generating commands for ", target_type_map_inverse.at(type), ": ", name, ".\n");

  string_set header_directories;
  string_set library_directories;
  string_vector dependency_libraries;

  gather_dependency_information(header_directories, library_directories, dependency_libraries);

  // add the project's header dirs
  for(auto&& directory : directories.at(file_type::header))
  {
    header_directories.insert(configuration.source_directory / directory);
  }

  // Command generation
  toolchain_option_map toolchain_options = ::libambrosia::toolchain_options.at(configuration.target_toolchain);
  os_option_map os_options = ::libambrosia::os_options.at(configuration.target_os);
  generate_parallel_commands(toolchain_options, os_options, header_directories);
  generate_final_commands(library_directories,
                          dependency_libraries);

  debug(debug::command_gen, "binary::generate_commands::Final commands: ", final_commands, "\n");
}

void binary::dump_commands() const
{
  debug(debug::command_gen, "binary::dump_commands::Parallel commands: ", parallel_commands, "\n");

  debug(debug::command_gen, "binary::dump_commands::Final commands: ", final_commands, "\n");
}

void binary::execute_build_commands() const
{
  debug(debug::command_exec, "binary::execute_build_commands::Building ", target_type_map_inverse.at(type), ": ", name, ".\n");
  debug(debug::command_exec, "binary::execute_build_commands::Creating build directory: ", configuration.build_directory, "\n");
  platform::create_directory_recursive(configuration.build_directory);

  for(auto&& command : parallel_commands)
  {
    string stdout_output;
    string stderr_output;
    auto result = execute_command(command, stdout_output, stderr_output);
    if(!result.first)
    {
      debug(debug::command_exec, "binary::execute_build_commands::failed to execute command: ", command, "\n");
      throw internal_error("Failed to execute build command.");
    }
    debug(debug::command_exec, "binary::execute_commands::Command executed succesfully, returned: ", result.second, ".\n");
    if(result.second != 0)
    {
      debug(debug::command_exec, "binary::execute_commands::Command returned failure.\n");
      throw command_error(stderr_output, command);
    }
    debug(debug::command_exec, "binary::execute_build_commands::Command execution succesful:\n",
                               "\tcommand was: ", command, "\n",
                               "\tstdout: ", stdout_output, "\n",
                               "\tstderr: ", stderr_output, "\n");
  }
  for(auto&& command : final_commands)
  {
    string stdout_output;
    string stderr_output;
    auto result = execute_command(command, stdout_output, stderr_output);
    if(!result.first)
    {
      debug(debug::command_exec, "binary::execute_build_commands::failed to execute command: ", command, "\n");
      throw internal_error("Failed to execute link command.");
    }
    if(result.second != 0)
    {
      debug(debug::command_exec, "binary::execute_commands::Command returned failure.\n", stderr_output, "\n");
      throw command_error(stderr_output, command);
    }
    debug(debug::command_exec, "binary::execute_build_commands::Link command execution succesful!\n",
                               "\tcommand was: ", command, "\n",
                               "\tstdout: ", stdout_output, "\n",
                               "\tstderr: ", stderr_output, "\n");
  }
}

void binary::gather_dependency_information(string_set& header_directories,
                                           string_set& library_directories,
                                           string_vector& libraries)
{
  // Dependency information gathering
  debug(debug::command_gen, "binary::gather_dependency_information::Gathering information from target dependencies: ", dependencies[target_type::library].size(), " library dependencies.\n");
  for(auto&& dependency : dependencies[target_type::library])
  {
    debug(debug::command_gen, "binary::gather_dependency_information::Including dependency ", dependency->name, "\'s header directories.\n");
    const string& source_directory = dependency->source_directory();
    const string_set& dependency_header_directories = dependency->directories.at(file_type::header);
    if(dependency->type == target_type::external)
    {
      // dependency_header_directories contains full paths (as e.g. specified on commandline)
      debug(debug::command_gen, "binary::gather_dependency_information::Including external dependency ", dependency->name, "'s header directories:\n", dependency_header_directories);
      for(auto&& header_directory : dependency_header_directories)
      {
        header_directories.insert(header_directory);
      }
    }
    else
    {
      debug(debug::command_gen, "binary::gather_dependency_information::Including internal dependency ", dependency->name, "'s header directories:\n");
      for(auto&& header_directory : dependency_header_directories)
      {
        debug(debug::command_gen, "binary::gather_dependency_information::Including directory: ", header_directory, ", with source directory ", source_directory, "\n");
        header_directories.insert(source_directory / header_directory);
      }
    }
    debug(debug::command_gen, "binary::gather_dependency_information::Including library ", dependency->name, " in (dynamic) linker command with library search directories:\n",
                              dependency->directories[file_type::library],
                              "\tand library names\n", dependency->libraries);
    libraries.push_back(dependency->name); // should use dependency->configuration.name, but that requires a cast as class target has no member configuration :(
    for(auto&& library_directory : dependency->directories[file_type::library])
    {
      library_directories.insert(library_directory);
    }

    for(auto&& library : dependency->libraries)
    {
      libraries.push_back(library);
    }
    debug(debug::command_gen, "binary::gather_dependency_information::Gathering dependency ", dependency->name, "\'s dependencies\' dependency information.\n");
    dependency->gather_dependency_information(header_directories, library_directories, libraries);
  }
}

void binary::generate_parallel_commands(const toolchain_option_map& toolchain_options,
                                        const os_option_map& /*os_options*/,
                                        const string_set& header_directories)
{
  file_set& object_files = files[file_type::object]; // for easy access
  for(auto&& type_files : files)
  {
    file_type type = type_files.first;
    if(get_general_type(type) == file_type::source)
    {
      const file_set& source_files = type_files.second;
      const language_option_map language_options = ::libambrosia::language_options.at(configuration.target_toolchain).at(type);
      debug(debug::command_gen, "binary::generate_parallel_commands::Generating commands for ", source_files.size(), " ", file_type_map_inverse.at(type), " files.\n");
      platform::command first_part; // before source file name
      platform::command second_part; // between source file name and object file name
      platform::command third_part; // after object file name

      // generate the part of the command that comes before the source file name
      debug(debug::command_gen, "binary::generate_parallel_commands::Generating first part of command.\n");

      if(type == file_type::source_c)
        first_part.set_program(toolchain_options.at(toolchain_option::compiler_c));
      else if(type == file_type::source_cxx)
        first_part.set_program(toolchain_options.at(toolchain_option::compiler_cxx));
      else if(type == file_type::source_fortran)
        first_part.set_program(toolchain_options.at(toolchain_option::compiler_fortran));
      else
        throw error("Compiling source files other than C, C++, or Fortran is unsupported at this time.");

      debug(debug::command_gen, "binary::generate_parallel_commands::getting language standard option.\n");
      //first_part.add_argument(language_options.at(language_option::compile_language));
      if(!contains(configuration.config_strings, "msvc"))
        first_part.add_argument(configuration.languagestd_option(language_options, type));
      first_part.add_argument(toolchain_options.at(toolchain_option::compiler_nologo));
      first_part.add_argument(toolchain_options.at(toolchain_option::compile_only));
      for(auto&& header_directory : header_directories)
      {
        first_part.add_argument(toolchain_options.at(toolchain_option::include_dir) + header_directory);
      }

      debug(debug::command_gen, "binary::generate_parallel_commands::adding DEFINES.\n");
      for(auto&& define : configuration.defines)
      {
        first_part.add_argument(toolchain_options.at(toolchain_option::define) + define);
      }

      // generate the part of the command that comes between the source file name and object file name
      debug(debug::command_gen, "binary::generate_parallel_commands::Generating second part of command.\n");
      // nada

      // generate part of the command that comes after the object file name
      debug(debug::command_gen, "binary::generate_parallel_commands::Generating third part of command.\n");
      // nada
      for(auto&& source_file : source_files)
      {
        const string object_filename = configuration.build_directory / get_basename(source_file.name) + toolchain_options.at(toolchain_option::object_extension);
        const auto result = object_files.insert(file(object_filename, platform::last_modified(object_filename)));
        if(!result.second)
          throw internal_error("Object file with the exact same name encountered. I really need to handle this better.");
        if(std::difftime(source_file.time_modified, result.first->time_modified) < 0)
          continue; // object file newer, skip command generation and execution

        platform::command command = first_part;
        command.add_argument(source_file.name);
        command.add_arguments(second_part);
        command.add_argument(toolchain_options.at(toolchain_option::output_object) + object_filename);
        command.add_arguments(third_part);
        debug(debug::command_gen, "binary::generate_parallel_commands::Command: ", command, "\n");
        parallel_commands.push_back(command);
      }
    }
    else
      debug(debug::command_gen, "binary::generate_parallel_commands::Skipping ", file_type_map_inverse.at(type_files.first), " files in command generation.\n");
  }
}

void binary::generate_final_commands(const string_set& library_directories,
                                     const string_vector& dependency_libraries)
{
  time_t time_newest_object_file = std::max_element(std::begin(files.at(file_type::object)), std::end(files.at(file_type::object)), [](const file& f1, const file& f2) { return f1.time_modified < f2.time_modified; })->time_modified;
  // TODO: handle link library modified dates, currently always relinking...
  platform::command link_command;
  if(type == target_type::library)
  {
    const string library_name = configuration.build_directory / (toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_prefix) + configuration.name + toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_library_extension));
    debug(debug::always, "\t", library_name, "\n");
    if(platform::last_modified(library_name) < time_newest_object_file)
      return; // skip library linking if output file is newer than all object files

    //TODO: check static vs shared library
    link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_linker));
    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::static_link_options) + library_name);
  }
  else if(type == target_type::application)
  {
    const string application_name = configuration.build_directory / configuration.name  + os_options.at(configuration.target_os).at(os_option::executable_extension);
    //See TODO above
    //if(platform::last_modified(application_name) > time_newest_object_file)
    //  return; // skip application linking
    if(contains(configuration.source_types, file_type::source_cxx))
    {
      debug(debug::command_gen, "binary::generate_commands::Linking with C++ linker driver.\n");
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_cxx));
      if(contains(configuration.source_types, file_type::source_fortran))
        libraries.insert(std::begin(libraries), toolchain_options.at(configuration.target_toolchain).at(toolchain_option::runtime_library_fortran));
    }
    else if(contains(configuration.source_types, file_type::source_fortran))
    {
      debug(debug::command_gen, "binary::generate_commands::Linking with Fortran linker driver.\n");
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_fortran));
    }
    else
    {
      debug(debug::command_gen, "binary::generate_commands::Linking with C linker driver.\n");
      link_command.set_program(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::dynamic_linker_c));
    }

    link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::output_object)+application_name);
  }
  for(auto&& object_file : files.at(file_type::object))
  {
    link_command.add_argument(object_file.name);
  }
  if(type == target_type::application) // should also enable this for shared libraries
  {
    // add all library search directories
    for(auto&& library_directory : library_directories)
    {
      link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::link_search_directory) + library_directory);
    }
    // add all libraries to link
    for(auto&& library : dependency_libraries)
    {
      link_command.add_argument(toolchain_options.at(configuration.target_toolchain).at(toolchain_option::link_library) + library);
    }
  }
  final_commands.push_back(link_command);
}

} // namespace lib

} // namespace ambrosia
