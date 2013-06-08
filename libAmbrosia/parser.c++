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
 * parser.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/parser.h++"

// libAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/Error/error.h++"
#include "Ambrosia/Error/syntax_error.h++"
#include "Ambrosia/dependency_paths.h++"
#include "Ambrosia/lexer.h++"
#include "Ambrosia/Target/binary.h++"
#include "Ambrosia/Target/project.h++"

// C++ includes
#include <iostream>
  using std::cerr;
  using std::istream;
#include <memory>
  using std::unique_ptr;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

parser::parser(libambrosia::project& project,
               const std::string& filename,
               const std::string& subdirectory,
               const dependency_paths_set& external_dependencies,
               const dependency_map& internal_dependencies)
: project(project),
  filename(filename),
  subdirectory(subdirectory),
  external_dependencies(external_dependencies),
  internal_dependencies(internal_dependencies)
{   }

void parser::syntax_warning(const string& message,
                            const size_t line_number,
                            const string_vector& warning_list) const
{
  debug(debug::status, "parser::syntax_warning::Emitting a syntax warning now.\n");
  cerr << "\nSyntax warning: " + filename + "\n" +
          "       line " + to_string(line_number) + "\n" +
          "       " + message << "\n";
  for(auto&& item : warning_list)
  {
    cerr << "\n\t" << item;
  }
}

void parser::parse_nectar()
{
  debug(debug::nectar_parser, "parser::parse::Parsing file: ", filename, ".\n");

  const auto& stream_ptr(platform::open_ifstream(filename));
  auto& stream = *stream_ptr;
  if(!stream)
    throw error("Unable to open *.nectar.txt file: " + filename);

  debug(debug::parser, "parser::parse_nectar::Opening file: ", filename, " succeeded, parsing.\n");

  lexer lexer(stream, filename);

  // Remove leading BOM
  skip_BOM(stream, filename);

  // Set build directory
  if(project.configuration.source_directory == ".")
    project.configuration.build_directory = "./build";

  bool global_processed = false;
  string token;
  while(lexer.next_token(token))
  {
    debug(debug::parser, "parser::parse::processing token: \'", token, "\'.\n");

    if("(" == token)
      lexer.process_outer_conditional(project.configuration);
    else if("global" == token)
    {
      debug(debug::parser, "parser::parse::global section found at line ", lexer.line_number, ".\n");
      if(global_processed)
        throw syntax_error("Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed.", filename, lexer.line_number);

      global_processed = true;
      if(lexer.next_token(token) && "{" == token)
        parse_global(lexer);
      else
        throw syntax_error("\'global\' must be followed by \'{\'.", filename, lexer.line_number);
    }
    else if("sub" == token)
    {
      debug(debug::parser, "parser::parse::Subproject section found at line ", lexer.line_number, ".\n");
      // get name and dependencies of sub target
      if(lexer.next_token(token))
      {
        debug(debug::parser, "parser::parse::Processing subproject ", token, ".\n");
        // Search for sub-project file: sourcedir/token/token.nectar.txt
        // 1. check for subdirectory
        const string full_subproject_directory = project.configuration.source_directory / subdirectory / token;
        if(!platform::directory_exists(full_subproject_directory))
          throw nectar_error("Directory " + full_subproject_directory + " not found.\n"
                             "Subproject names must be identical to the subproject directory.", filename, lexer.line_number );

        string subproject_filename = token + ".nectar.txt";
        string full_subproject_filename = full_subproject_directory / subproject_filename;
        if(!platform::file_exists(full_subproject_filename))
          throw nectar_error("Subproject project file missing: " + full_subproject_filename, filename, lexer.line_number);

        dependency_map subproject_dependencies = read_dependencies(lexer);
        debug(debug::parser, "parser::parse::Subproject dependencies:\n", subproject_dependencies, "\n");

        // copy configuration and set proper subdirectory
        configuration subconfiguration = project.configuration;
        debug(debug::config, "parser::parse::Setting source directory of subproject ", token, " to ", full_subproject_directory, ".\n");
        subconfiguration.source_directory = full_subproject_directory;
        debug(debug::config, "parser::parse::Setting build directory of subproject ", token, " to ", project.configuration.build_directory / token, ".\n");
        subconfiguration.build_directory = project.configuration.build_directory / token;

        unique_ptr<libambrosia::project> subproject(new libambrosia::project(project.name + "::" + token, subconfiguration));

        parser subproject_parser(*subproject, full_subproject_filename, subdirectory / token, external_dependencies, subproject_dependencies);
        subproject_parser.parse_nectar();

        project.targets.emplace_back(std::move(subproject));
      }
      else
        throw syntax_error("\'sub\' must be followed by the name of the subproject.", filename, lexer.line_number);
    }
    else if("app" == token || "lib" == token)
    {
      const target_type type = target_type_map.at(token);

      debug(debug::parser, "parser::parse::", token, " section found at line ", lexer.line_number, ".\n");
      if(lexer.next_token(token))
      {
        if("{" == token)
          throw syntax_error("Expected " + token + " name after \'" + token + "\'.", filename, lexer.line_number);
        else
        {
          // Store target name
          const string target_name = token;

          // Add target's dependencies
          dependency_map dependencies = read_dependencies(lexer);

          if(lexer.next_token(token) && token == "{")
          {
            // parse the target for each configuration to be built
            istream::pos_type position_after_target;
            size_t line_number_after_target;
            size_t column_number_after_target;
            for(auto&& build_type : project.configuration.build_types)
            {
              for(linkage linkage : project.configuration.build_linkages)
              {
                debug(debug::parser, "parser::parse::Parsing target ", target_name, " for a ", linkage_map_inverse.at(linkage), " ", build_type_map_inverse.at(build_type), " build.\n");

                unique_ptr<binary> binary_target(new binary(target_name, type, build_type_map_inverse.at(build_type), linkage_map_inverse.at(linkage), project.configuration, project.files, project.directories, dependencies));

                const auto& stream_ptr(platform::open_ifstream(filename));
                auto& target_stream = *stream_ptr;
                if(!target_stream)
                  throw error("Unable to open *.nectar.txt file: " + filename);

                //TODO use a copy constructor
                ::libambrosia::lexer target_lexer(target_stream, filename, lexer.line_number, lexer.column_number, stream.tellg());
                parse_binary(*binary_target, target_lexer);

                project.targets.emplace_back(std::move(binary_target));

                debug(debug::parser, "parser::parse::Finished with ", target_name, " for a ", linkage_map_inverse.at(linkage), " ", build_type_map_inverse.at(build_type), " build.\n");
                position_after_target = target_lexer.stream.tellg();
                line_number_after_target = target_lexer.line_number;
                column_number_after_target = target_lexer.column_number;
              }
            }
            lexer.stream.seekg(position_after_target);
          }
          else
            throw syntax_error("Expected \'{\' after target name or dependency list.", filename, lexer.line_number);
        }
      }
    }
    else if("dep" == token)
    {
      debug(debug::parser, "parser::parse::dep section found at line ", lexer.line_number, ".\n");
      if(lexer.next_token(token))
      {
        debug(debug::parser, "parser::parse::Processing dependency of type ", token, ".\n");
        target_type dependency_type = target_type_map.at(token);
        if(lexer.next_token(token))
        {
          const string name = token;
          debug(debug::parser, "parser::parse::Resolving dependency target: ", name, ".\n");

          parse_dependency(name, dependency_type, external_dependencies, lexer);
        }
        else
          throw syntax_error("Expected dependency name after dependency type.", filename, lexer.line_number);
      }
      else
        throw syntax_error("Expected dependency type (\'app\' or \'lib\') after \'dep\'.", filename, lexer.line_number);
    }
    else
      throw syntax_error("Unexpected token: \'" + token + "\'. Expected global, sub, app, lib, dep, or test.", filename, lexer.line_number);
  }
  debug(debug::nectar_parser, "parser::Finished with file: ", filename, ".\n");
}

void parser::parse_file_list(const file_type type,
                             target& target,
                             configuration& configuration,
                             lexer& lexer)
{
  debug(debug::parser, "parser::parse_file_list::Parsing ", file_type_map_inverse.at(type), " file list.\n");
  bool empty = true; // a list must not be empty
  string token;

  while(lexer.next_list_token(configuration, token))
  {
    debug(debug::parser, "parser::parse_file_list::adding matches to file ", token, " to the file list.\n");
    empty = false;
    add_source_file(target, type, token, configuration);
  }
  if(empty)
    throw syntax_error("A list must not be empty.", filename, lexer.line_number);
}
void parser::parse_source_directory_list(const file_type type,
                                         target& target,
                                         const configuration& configuration,
                                         lexer& lexer)
{
  debug(debug::parser, "parser::parse_source_directory_list::Parsing full list, nonexistent directories are kept in error_list.\n");
  bool empty_list = true; // a list must not be empty
  string token;
  string_vector error_list;
  // gather all list items
  while(lexer.next_list_token(configuration, token))
  {
    empty_list = false;

    if(!add_source_directory(target, type, token, configuration))
      error_list.push_back("line " + to_string(lexer.line_number) +": " + token); // add the bad directory to error_list
  }
  if(empty_list)
    throw syntax_error("A source directory list must not be empty", filename, lexer.line_number);
  else if(!error_list.empty())
    throw nectar_error("Some source directories were not found:", filename, lexer.line_number, error_list);
}
void parser::parse_build_directory(const file_type,
                                   lexer& lexer)
{
  throw nectar_error("Build directory list parsing isn't done yet.", filename, lexer.line_number);
}
void parser::parse_variable_list(string_set& strings,
                                 const configuration& configuration,
                                 lexer& lexer)
{
  string token;
  while(lexer.next_list_token(configuration, token))
  {
    if(token[0] == '~')
    {
      auto it = strings.find(token);
      if(it != std::end(strings))
        strings.erase(it);
      else
        syntax_warning("String " + token + " not present in variable.", lexer.line_number);
    }
    else
    {
      if(!strings.insert(token).second)
        syntax_warning("String " + token + " already in variable.", lexer.line_number);
    }
  }
  //throw nectar_error("Variable list parsing isn't done yet.", filename, lexer.line_number);
}

void parser::parse_library_list(target& target,
                                const configuration& configuration,
                                lexer& lexer)
{
  string token;
  string_vector error_list;
  while(lexer.next_list_token(configuration, token))
  {
    // LIBS items must be of the form '-lsomelib' or '-Lsomedirectory'
    if(token.size() <= 2)
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", filename, lexer.line_number);
    else if(!token.compare(0, 2, "-l"))
    {
      token = token.substr(2);
      debug(debug::parser, "parser::parse_library_list::Found library name: ", token, ".\n");
      add_library(target, token);
    }
    else if(!token.compare(0, 2, "-L"))
    {
      token = token.substr(2);
      debug(debug::parser, "parser::parse_library_list::Found library search directory: ", token, ".\n");
      if(!platform::directory_exists(token))
        error_list.push_back(token);
      if(platform::is_absolute_path(token))
      {
        debug(debug::parser, "parser::parse_library_list::Absolute library path detected.\n");
        syntax_warning("Absolute paths in project files should be avoided.", lexer.line_number);
      }
    }
    else
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", filename, lexer.line_number);
  }
}

void parser::add_source_file(target& target,
                                    const file_type general_type,
                                    const string& filename,
                                    configuration& configuration)
{
  // add source file type to list
  // search specific file_type directories
  const file_type specific_type = detect_type(general_type, filename);
  string_set& specific_directories = target.directories[specific_type];
  string_set& general_directories = target.directories[general_type];
  specific_directories.insert(std::begin(general_directories), std::end(general_directories));
  debug(debug::parser, "parser::add_source_file::Finding ", file_type_map_inverse.at(specific_type), " files matching ", filename, " in:\n", specific_directories);
  project.file_cache.find_source_files(filename, configuration.source_directory, specific_directories, target.files[specific_type]);
  configuration.source_types.insert(specific_type);
}
bool parser::add_source_directory(target& target,
                                         const file_type type,
                                         const string& directory,
                                         const configuration& configuration)
{
  debug(debug::parser, "parser::add_source_directory::Adding directory ", directory, " of type ", file_type_map_inverse.at(type), ".\n");
  if(!project.file_cache.add_source_directory(configuration.source_directory / directory))
    return false;
  if(!target.directories[type].insert(directory).second)
    debug(debug::parser, "parser::add_source_directory::Directory ", directory, " already present.\n");

  const file_type general_type = get_general_type(type);
  if(type != general_type)
  {
    debug(debug::parser, "parser::add_source_directory::Adding directory ", directory, " of general type ", file_type_map_inverse.at(type), ".\n");
    if(!target.directories[general_type].insert(directory).second)
      debug(debug::parser, "parser::add_source_directory::Directory ", directory, " already present.\n");
  }
  return true;
}

void parser::add_library(target& target,
                                const string& library)
{
  target.libraries.insert(library);
}

void parser::parse_global(lexer& lexer)
{
  debug(debug::parser, "parser::parse_global::Processing global section.\n");
  size_t curly_brace_count = 1; // parsing starts inside curly braces block
  string token;
  bool already_modified_NAME = false;
  while(curly_brace_count > 0 && lexer.next_token(token))
  {
    debug(debug::parser, "parser::parse_global::Token: \'", token, "\', curly_brace_count: ", curly_brace_count, ".\n");
    if("}" == token)
      --curly_brace_count;
    else if("{" == token)
      ++curly_brace_count;
    else if("(" == token)
      lexer.process_inner_conditional(project.configuration);
    else if("CONFIG" == token)
      // Fix this: not everything is possible in global sections
      parse_variable_list(project.configuration.config_strings, project.configuration, lexer);
    else if ("NAME" == token)
    {
      if(!lexer.next_list_token(project.configuration, token))
        throw syntax_error("Expected project name after NAME.", filename, lexer.line_number);

      project.configuration.name = token;
      if(lexer.next_list_token(project.configuration, token))
        throw syntax_error("NAME must be followed by only one token.", filename, lexer.line_number);
    }
    else
    {
      file_type type;
      if(map_value(file_type_map, token, type))
      {
        debug(debug::parser, "parser::parse_global::", token, " list detected.\n");
        if(type == file_type::library)
          parse_library_list(project, project.configuration, lexer);
        else
          parse_file_list(type, project, project.configuration, lexer);

        debug(debug::parser, "parser::parse_global::Succesfully parsed list of files or librar(y director)ies.\n");
      } // or a list of directories
      else if(map_value(directory_type_map, token, type))
      {
        debug(debug::parser, "parser::parse_global::", file_type_map_inverse.at(type), " directory list detected.\n");
        const file_type general_type = get_general_type(type);
        if(general_type == file_type::source || type == file_type::header)
          parse_source_directory_list(type, project, project.configuration, lexer);
        else
          throw nectar_error("Parsing of " + token + " not yet implemented.", filename, lexer.line_number);
      }
      else
        throw syntax_error("Unexpected token: " + token, filename, lexer.line_number);
    }
  }
}

void parser::parse_binary(binary& binary,
                          lexer& lexer)
{
  debug(debug::parser, "parser::parse_binary::Processing named target section: ", binary.name, ".\n");
  size_t curly_brace_count = 1; // parsing starts inside curly braces block
  string token;
  bool already_modified_NAME = false;

  while(curly_brace_count > 0 && lexer.next_token(token))
  {
    debug(debug::parser, "parser::parse_binary::Token: \'", token, "\', line number ", lexer.line_number, ", curly_brace_count: ", curly_brace_count, ".\n");
    if("}" == token)
      --curly_brace_count;
    else if("{" == token)
      ++curly_brace_count;
    else if("(" == token)
      lexer.process_inner_conditional(binary.configuration);
    else if("CONFIG" == token)
      parse_variable_list(binary.configuration.config_strings, binary.configuration, lexer);
    else if ("DEFINES" == token)
    {
      debug(debug::parser, "parser::parse_binary::DEFINES detected.\n");
      bool empty_list = true;
      while(lexer.next_list_token(binary.configuration, token))
      {
        empty_list = false;
        debug(debug::parser, "parser::parse_binary::Adding \'", token, "\' to list of DEFINES.\n");
        if(!binary.configuration.defines.insert(token).second)
          throw syntax_error("DEFINES already contains \'" +  token + "\'.", filename, lexer.line_number);
      }
      if(empty_list)
        throw syntax_error("\'DEFINES\' must be followed by a list of compiler defines (e.g. SOMEMACRO, SOMEMACRO=somevalue).", filename, lexer.line_number);
    }
    else if ("NAME" == token)
    {
      debug(debug::parser, "parser::parse_binary::NAME detected.\n");

      if(already_modified_NAME)
        syntax_warning("NAME is being modified twice in this target section.", lexer.line_number);

      already_modified_NAME = true;
      if(lexer.next_token(token, special_characters_newline))
      {
        if("\n" != token)
        {
          binary.configuration.name = token;
          continue;
        }
      }
      throw syntax_error("NAME must be followed by the target's output name (without prefix/suffix", filename, lexer.line_number );
    }
    else
    {
      file_type type;
      // is it a list of files or librar(y director)ies?
      if(map_value(file_type_map, token, type))
      {
        debug(debug::parser, "parser::parse_binary::", token, " list detected.\n");
        if(type == file_type::library)
          parse_library_list(binary, binary.configuration, lexer);
        else
          parse_file_list(type, binary, binary.configuration, lexer);

        debug(debug::parser, "parser::parse_binary::Succesfully parsed list of files or librar(y director)ies.\n");
      } // or a list of directories
      else if(map_value(directory_type_map, token, type))
      {
        debug(debug::parser, "parser::parse_binary::", file_type_map_inverse.at(type), " directory list detected.\n");
        const file_type general_type = get_general_type(type);
        if(general_type == file_type::source || type == file_type::header)
          parse_source_directory_list(type, binary, binary.configuration, lexer);
        else
          throw nectar_error("Parsing of " + token + " not yet implemented.", filename, lexer.line_number);
      }
      else
        throw syntax_error("Unexpected token: " + token, filename, lexer.line_number);
    }
  }
}

void parser::parse_dependency(const string& name,
                              const target_type type,
                              const dependency_paths_set& external_dependencies,
                              lexer& lexer)
{
  debug(debug::parser, "parser::parse_dependency::Checking availability of dependency ", name, ".\n");
  target* dependency = nullptr;
  const auto find_target = [&name,&type](const target* t)
  {
    debug(debug::parser, "parser::parse_dependency::find_target::Checking dependency: ", t->name, ".\n");
    return (name == t->name || wildcard_compare("*::"+name, t->name)) && type == t->type;
  };

  debug(debug::parser, "parser::parse_dependency::Searching in project ", project.name, "\'s dependencies:\n", internal_dependencies, "\n");
  auto dependencies = internal_dependencies.find(type);
  if(dependencies != std::end(internal_dependencies))
  {
    debug(debug::parser, "parser::parse_dependency::Finding match in ", dependencies->second.size(), " internal dependencies.\n");
    auto result = std::find_if(std::begin(dependencies->second), std::end(dependencies->second), find_target);
    if(result != std::end(dependencies->second))
    {
      debug(debug::parser, "parser::parse_dependency::Found dependency in project ", project.name, ", so it should be OK.\n");
      dependency = *result;
    }
  }
  if(dependency == nullptr)
  {
    debug(debug::parser, "parser::parse_dependency::Didn't find dependency in current project, checking if it's an external dependency.\n");
    auto result = external_dependencies.find(name);
    if(result == std::end(external_dependencies))
      throw nectar_error("Required dependency not found: " + name, filename, lexer.line_number);

    debug(debug::parser, "parser::parse_dependency::Checking basic dependency contents.\n");
    if(type == target_type::library && (result->headers.empty() || result->libraries.empty()))
      throw nectar_error("Required library dependency "+name+" must have at least one header or library specified.", filename, lexer.line_number);
    if(type == target_type::application && (result->applications.empty()))
      throw nectar_error("Required application dependency "+name+" must have binary directory specified.", filename, lexer.line_number);

    debug(debug::parser, "Copying dependency directories to \'external\' target.\n");
    project.targets.emplace_back(new external(name, type, result->include, result->lib, result->bin));
    dependency = project.targets.back().get();
  }
  if(dependency == nullptr)
    throw nectar_error("required dependency was not found: " + name, filename, lexer.line_number);

  size_t curly_brace_count = 1; // parsing starts inside curly braces block
  string token;

  if(!lexer.next_token(token))
    throw syntax_error("Unexpected end of file: a \'dep\' target at the end of a nectar.txt file has no effect.", filename, lexer.line_number);
  if(token != "{")
  {
    debug(debug::parser, "parser::parse_dependency::No \'dep\' body found.\n");
    lexer.previous_token();
    curly_brace_count = 0; // skip the nonexistent dep body parsing
  }
  debug(debug::parser, "parser::parse_dependency::Parsing dependency body.\n");
  while(curly_brace_count > 0 && lexer.next_token(token))
  {
    if("}" == token)
      --curly_brace_count;
    else if("{" == token)
      ++curly_brace_count;
    else if("(" == token)
      lexer.process_inner_conditional(project.configuration); // uses local project configuration
    else if("HEADERS" == token)
    {
      debug(debug::parser, "parser::parse_dependency::Checking headers required by dependency ", name, ".\n");
      while(lexer.next_list_token(project.configuration, token))
      {
        debug(debug::parser, "parser::parse_dependency::Checking if header ", token, " is usable.\n");
        syntax_warning("Checking header usability is unimplemented: assuming header " + token + " is present and usable.", lexer.line_number);
      }
    }
    else
      throw syntax_error("Unexpected token: " + token, filename, lexer.line_number);
  }
  debug(debug::parser, "parser::parse_dependency::Added dependency ", dependency->name, " to project ", project.name, ".\n");
  project.dependencies[type].insert(dependency);
}

dependency_map parser::read_dependencies(lexer& lexer)
{
  debug(debug::parser, "parser::read_dependencies::Reading dependencies.\n");
  dependency_map dependencies;
  bool in_list = false;
  target_type type = target_type::library; // dummy to shut up GCC warning
  string token;
  while(lexer.next_token(token, special_characters_newline))
  {
    debug(debug::parser, "parser::read_dependencies::token: \'", output_form(token), "\'\n");
    if("{" == token || "\n" == token)
      break; // finished
    else if(!in_list)
    {
      if(":" == token)
      {
        if(lexer.next_token(token) && (token == "lib" || token == "app"))
        {
          type = target_type_map.at(token);
          debug(debug::parser, "parser::read_dependencies::Detected ", token, " dependencies.\n");
          in_list = true;
          goto insert_dependency;
        }
        else
          throw syntax_error("Expected list of target names after \':\'.", filename, lexer.line_number);
      }
      else
        throw syntax_error("Expected target body \'{\' or dependency list \':\'.", filename, lexer.line_number);
    }
    else if("," == token)
    {
      insert_dependency:
      if(lexer.next_token(token))
      {
        if("(" == token)
        {
          lexer.process_dependency_set_conditional(project.configuration);

          if(!lexer.next_token(token))
            throw syntax_error("Expected dependency name after dependency type", filename, lexer.line_number);
          if("{" == token)
          {
            lexer.previous_token(); // extract_nectar expects an opening brace here.
            break;
          }
        }
        const string& name = token;
        debug(debug::parser, "parser::read_dependencies::Locating ", target_type_map_inverse.at(type), " dependency: ", name, ".\n");
        if(!find_dependencies(dependencies, project, type, name))
          throw nectar_error("Dependency not declared in current project file: " + name + ". Make sure there is a matching \'dep\' item for this dependency, or check the name if it is a lib/app target in the current project.", filename, lexer.line_number);
      }
    }
    else
      in_list = false;
  }
  debug(debug::parser, "parser::read_dependencies::Finished reading ", dependencies.size(), " dependencies.\n");
  return dependencies;
}

} // namespace lib

} // namespace ambrosia
