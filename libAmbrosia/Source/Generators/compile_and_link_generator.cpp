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
 * Ambrosia/Generators/compile_and_link_generator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Generators/compile_and_link_generator.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/build_element.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"
#include "Ambrosia/Targets/project.h"
#include "Ambrosia/Targets/target.h"

// C++ includes
#include <sstream>
  using std::ostringstream;
#include <string>
  using std::string;

libambrosia_namespace_begin

compile_and_link_generator::compile_and_link_generator(const file_type type,
                                                       const target& target)
: generator(type, target)
{
  debug(debug::command_gen) << "compile_and_link_generator::compile_and_link_generator::Generator created for output in \""
                            << target.configuration.build_directory << "\"\n";
}

compile_and_link_generator::~compile_and_link_generator()
{   }

void compile_and_link_generator::generate_object_filenames()
{
  debug(debug::command_gen) << "compile_and_link_generator::Generating object filenames for " << m_target.name << " that will be built in "
                            << "\"" << m_target.configuration.build_directory << "\".\n";
  const build_element_set& files = m_target.files.at(m_type);
  for(auto&& it = std::begin(files); it != std::end(files); ++it)
  {
    const build_element& current = *it;
    current.object_file.name = full_directory_name(m_target.configuration.build_directory, get_basename(current.source_file.name))
                               + m_toolchain_options.at(toolchain_option::object_extension);
    debug(debug::command_gen) << "compile_and_link_generator::generate_object_filenames::object file: " << current.object_file.name << "\n";
  }
}

const string_vector compile_and_link_generator::generate_parallel_commands()
{
  string_vector commands;
  ostringstream command;
  string languagestd;
  if(!contains(m_target.configuration.config_strings, "msvc"))
  {
    switch(m_type)
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

  for(auto&& it = std::begin(m_target.files.at(m_type)); it != std::end(m_target.files.at(m_type)); ++it)
  {
    string_pair split_name = split_preceding_directory(it->object_file.name);
    platform::create_directory_recursive(split_name.first);

    // compiler (e.g. 'gcc')
    command << m_toolchain_options.at(toolchain_option::compiler);
    // language standard
    if(!languagestd.empty())
      command << " " << languagestd;
    // compile argument (e.g. '-c')
    const string& compile_argument = m_toolchain_options.at(toolchain_option::compile_option);
    if(!compile_argument.empty())
      command << " " << compile_argument;
    // source file
    command << " " << it->source_file.name;
    // output argument (e.g. '-o')
    const string& output_argument = m_toolchain_options.at(toolchain_option::output_object);
    if(!output_argument.empty())
      command << " " << output_argument;
    // object filename
    command << " " << it->object_file.name;
    // include directories
    const string_set& header_dirs = m_target.source_directories.at(file_type::header);
    for(auto&& it = std::begin(header_dirs); it != std::end(header_dirs); ++it)
    {
      command << " " << m_toolchain_options.at(toolchain_option::include_option) << "\"" << full_directory_name(m_target.configuration.source_directory, *it) << "\"";
    }

    commands.push_back(command.str());
    debug(debug::command_gen) << "compile_and_link_generator::generate_parallel_commands::command: " << command.str() << "\n";
    command.str("");
  }

  return commands;
}

const string_vector compile_and_link_generator::generate_final_commands()
{
  return string_vector();
}

libambrosia_namespace_end
