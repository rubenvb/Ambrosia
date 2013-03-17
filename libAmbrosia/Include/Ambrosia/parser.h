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
 * Ambrosia/parser.h
 * Parser for ambrosia project files.
 * Parses sections multiple times to accomodate possible configuration specific differences
 *
 **/

#ifndef AMBROSIA_PARSER_H
#define AMBROSIA_PARSER_H

// Global include
#include "Ambrosia/global.h"

// Ambrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

namespace ambrosia
{
namespace lib
{

// Forward declarations
class configuration;
class lexer;
class project;

class parser
{
public:
  parser(libambrosia::project& project,
         const std::string& full_filename,
         const std::string& sub_directory,
         const dependency_paths_set& external_dependencies,
         const dependency_map& internal_dependencies = {});

  void syntax_warning(const std::string& message,
                      const std::size_t line_number,
                      const string_vector& warning_list = {}) const;

  void parse_nectar();

  // item lists
  void parse_file_list(const file_type type,
                       target& target,
                       configuration& configuration,
                       lexer& lexer); // matches wildcards to filenames and checks existence
  void parse_source_directory_list(const file_type type,
                                   target& target,
                                   const configuration& configuration,
                                   lexer& lexer); // searches source_directory / subdirectory
  void parse_build_directory(const file_type type,
                             lexer& lexer);  // will be created on first use
  void parse_variable_list(string_set& items,
                           const configuration& configuration,
                           lexer& lexer); // only adds or removes strings from variables
  void parse_library_list(target& target,
                          const configuration& configuration,
                          lexer& lexer); // parses -l and -L items, and handle interproject dependencies?
  // Adding files
  void parse_library_list(target& target,
                          lexer& lexer);
  void add_source_file(target& target,
                       const file_type general_type,
                       const std::string& filename,
                       configuration& configuration);
  bool add_source_directory(target& target,
                            const file_type type,
                            const std::string& directory,
                            const configuration& configuration);
  void add_library(target& target,
                   const std::string& library);
  // target parsers
  void parse_global(lexer& lexer);
  void parse_binary(binary& binary,
                    lexer& lexer);
  void parse_dependency(const std::string& name,
                        const target_type type,
                        const dependency_paths_set& external_dependencies,
                        lexer& lexer);

  dependency_map read_dependencies(lexer& lexer);

private:
  ::libambrosia::project& project;
  const std::string& filename;
  const std::string subdirectory; // used for file searching, without configuration.source_directory() !
  const dependency_paths_set& external_dependencies;
  const dependency_map& internal_dependencies;
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_PARSER_H
