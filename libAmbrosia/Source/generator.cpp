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
 * Ambrosia/Generators/generator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/generator.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/boost_wrapper.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
#include <memory>
  using std::unique_ptr;
#include <sstream>
  using std::ostringstream;
#include <string>
  using std::string;

libambrosia_namespace_begin

generator::generator(const ::libambrosia::file_type type,
                     build_element_set& files,
                     const string_set& header_directories,
                     const ::libambrosia::configuration& configuration)
: type(type),
  files(files),
  header_directories(header_directories),
  configuration(configuration),
  toolchain_options(::libambrosia::toolchain_options.at(configuration.target_toolchain)),
  language_options(::libambrosia::language_options.at(configuration.target_toolchain).at(type)),
  os_options(::libambrosia::os_options.at(configuration.target_os))
{  }

generator::~generator()
{   }

void generator::generate_object_filenames()
{
  debug(debug::command_gen) << "generator::Generating object filenames for " << file_type_map_inverse.at(type) << " files that will be built in "
                            << "\"" << configuration.build_directory << "\".\n";

  for(auto it = std::begin(files); it != std::end(files); ++it)
  {
    const build_element& current = *it;
    //TODO: handle source files with the same name which would cause object files overwriting each other
    current.object_file.name = full_directory_name(configuration.build_directory, get_basename(current.source_file.name))
                               + toolchain_options.at(toolchain_option::object_extension);
    debug(debug::command_gen) << "generator::generate_object_filenames::object file: " << current.object_file.name << "\n";
  }
}

void generator::generate_parallel_commands(std::back_insert_iterator<command_vector> inserter)
{
  platform::command first_part;
  platform::command second_part;
  platform::command third_part;

  // generate the part of the command that comes before the source file name
  if(type == file_type::source_c)
    first_part.set_program(toolchain_options.at(toolchain_option::compiler_c));
  else if(type == file_type::source_cxx)
    first_part.set_program(toolchain_options.at(toolchain_option::compiler_cxx));
  else if(type == file_type::source_fortran)
    first_part.set_program(toolchain_options.at(toolchain_option::compiler_fortran));
  else
    throw error("Compiling source files other than C, C++, or Fortran is unsupported at this time.");

  //first_part.add_argument(language_options.at(language_option::compile_language));
  //first_part.add_argument(languagestd_option());
  first_part.add_argument(toolchain_options.at(toolchain_option::compile_only));
  for(auto&& it = std::begin(header_directories); it != std::end(header_directories); ++it)
  {
    first_part.add_argument(toolchain_options.at(toolchain_option::include_dir) + "\"" + full_directory_name(configuration.source_directory, *it) + "\"");
  }

  // generate the part of the command that comes between the source file name and object file name
  second_part.add_argument(toolchain_options.at(toolchain_option::output_object));

  // generate part of the command that comes after the object file name
  // nada

  debug(debug::command_gen) << "generator::generate_parallel_commands::Command template: " << first_part << " <source file> " << second_part << " <object_file> " << third_part << ".\n";

  for(auto&& it = std::begin(files); it != std::end(files); ++it)
  {
    platform::command command = first_part;
    command.add_argument(it->source_file.name);
    command.add_arguments(second_part);
    command.add_argument(it->object_file.name);
    command.add_arguments(third_part);

    // insert command string
    inserter = command;
    debug(debug::command_gen) << "generator::generate_parallel_commands::command: " << command << "\n";
  }
}

const string& generator::languagestd_option() const
{
  switch(type)
  {
    case file_type::source_c:
      if(contains(configuration.config_strings, "C89"))
        return language_options.at(language_option::std_c89);
      else if(contains(configuration.config_strings, "C90"))
        return language_options.at(language_option::std_c90);
      else if(contains(configuration.config_strings, "C99"))
        return language_options.at(language_option::std_c99);
      else if(contains(configuration.config_strings, "C11"))
        return language_options.at(language_option::std_c11);
      else if(contains(configuration.config_strings, "GNU89"))
        return language_options.at(language_option::std_gnu89);
      else if(contains(configuration.config_strings, "GNU90"))
        return language_options.at(language_option::std_gnu90);
      else if(contains(configuration.config_strings, "GNU99"))
        return language_options.at(language_option::std_gnu99);
      else if(contains(configuration.config_strings, "GNU11"))
        return language_options.at(language_option::std_gnu11);
      else
        return language_options.at(language_option::std_c99);
    case file_type::source_cxx:
      if(contains(configuration.config_strings, "C++98"))
        return language_options.at(language_option::std_cxx98);
      else if(contains(configuration.config_strings, "C++03"))
        return language_options.at(language_option::std_cxx03);
      else if(contains(configuration.config_strings, "C++11"))
        return language_options.at(language_option::std_cxx11);
      else if(contains(configuration.config_strings, "GNU++98"))
        return language_options.at(language_option::std_gnuxx98);
      else if(contains(configuration.config_strings, "GNU++03"))
        return language_options.at(language_option::std_gnuxx03);
      else if(contains(configuration.config_strings, "GNU++11"))
        return language_options.at(language_option::std_gnuxx11);
      else
        return language_options.at(language_option::std_cxx11);
    default:
      throw internal_error("language std options not fully implemented yet.");
  }
}

libambrosia_namespace_end
