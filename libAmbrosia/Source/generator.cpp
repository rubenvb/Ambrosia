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
#include "Ambrosia/generator_maps.h"
#include "Ambrosia/status.h"

// C++ includes
#include <memory>
  using std::unique_ptr;
#include <sstream>
  using std::ostringstream;
#include <string>
  using std::string;

libambrosia_namespace_begin

generator::generator(const file_type type,
                     build_element_set& files,
                     const string_set& header_directories,
                     const ::libambrosia::configuration& configuration)
: type(type),
  files(files),
  header_directories(header_directories),
  configuration(configuration),
  toolchain_options(::libambrosia::toolchain_options.at(configuration.target_toolchain)),
  language_options(::libambrosia::language_options.at(type)),
  os_options(::libambrosia::os_options.at(configuration.target_os))
{  }

generator::~generator()
{   }

void generator::generate_object_filenames()
{

  debug(debug::command_gen) << "compile_and_link_generator::Generating object filenames for " << file_type_map_inverse.at(type) << " files that will be built in "
                            << "\"" << configuration.build_directory << "\".\n";

  for(auto&& it = std::begin(files); it != std::end(files); ++it)
  {
    const build_element& current = *it;
    current.object_file.name = full_directory_name(configuration.build_directory, get_basename(current.source_file.name))
                               + toolchain_options.at(toolchain_option::object_extension);
    debug(debug::command_gen) << "compile_and_link_generator::generate_object_filenames::object file: " << current.object_file.name << "\n";
  }
}

const string_vector generator::generate_parallel_commands()
{
  string_vector commands;
  ostringstream command;
  string languagestd;
  if(!contains(configuration.config_strings, "msvc"))
  {
    switch(type)
    {
      case file_type::source_c:
        emit_warning("C language standard defaults to c99");
        languagestd = "-std=c99";
        break;
      case file_type::source_cxx:
        emit_warning("C++ language standard defaults to C++11");
        languagestd = "-std=c++0x";
        break;
      case file_type::source_fortran:
        emit_warning("Fortran language standard defaults to f2008");
        languagestd = "-std=f2008";
        break;
      default:
        break;
    }
  }

  for(auto&& it = std::begin(files); it != std::end(files); ++it)
  {
    string_pair split_name = split_preceding_directory(it->object_file.name);
    platform::create_directory_recursive(split_name.first);

    // compiler (e.g. 'gcc')
    command << toolchain_options.at(toolchain_option::compiler);
    // language standard
    if(!languagestd.empty())
      command << " " << languagestd;
    // compile argument (e.g. '-c')
    const string& compile_argument = toolchain_options.at(toolchain_option::compile_only);
    if(!compile_argument.empty())
      command << " " << compile_argument;
    // source file
    command << " " << it->source_file.name;
    // output argument (e.g. '-o')
    const string& output_argument = toolchain_options.at(toolchain_option::output_object);
    if(!output_argument.empty())
      command << " " << output_argument;
    // object filename
    command << " " << it->object_file.name;
    // include directories;
    for(auto&& it = std::begin(header_directories); it != std::end(header_directories); ++it)
    {
      command << " " << toolchain_options.at(toolchain_option::include_dir) << "\"" << full_directory_name(configuration.source_directory, *it) << "\"";
    }

    commands.push_back(command.str());
    debug(debug::command_gen) << "compile_and_link_generator::generate_parallel_commands::command: " << command.str() << "\n";
    command.str("");
  }

  return commands;
}

const string_vector generator::generate_final_commands()
{
  return string_vector();
}

libambrosia_namespace_end
