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
 * Parser/nectar_loader.h
 * Class that extracts principal information from the project file:
 *  - canonical tokenization
 *  - target names
 *  - dependencies
 *  - TODO: user options
 *
 **/

#ifndef AMBROSIA_NECTAR_LOADER_H
#define AMBROSIA_NECTAR_LOADER_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/nectar.h"

// C++ includes
#include <iosfwd>
#include <functional>
#include <utility>
#include <vector>

libambrosia_namespace_begin

// forward declarations
class target;
class file_cache;

// characters that are read as seperate tokens in Ambrosia .nectar.txt files.
extern const std::set<char> s_special_characters;
extern const std::set<char> s_special_characters_newline;

// main nectar_loader class
class nectar_loader
{
public:
  nectar_loader(project& project,
                const std::string& full_filename,
                const std::string& sub_directory,
                std::istream& stream,
                const dependency_set& list = dependency_set());
  ~nectar_loader();

  void extract_nectar();

  // Disallow copy(constructor)ing and assignment (shuts up warning of -Weffc++)
  nectar_loader& operator=(const nectar_loader&) DELETED_FUNCTION;
  nectar_loader(const nectar_loader&) DELETED_FUNCTION;

private:
  project& m_project;
  const std::string& m_filename; // used for error reporting
  const std::string m_subdirectory; // used for file searching, without configuration.source_directory() !
  std::istream& m_stream; // file input stream
  size_t m_line_number; // used for error reporting
  const dependency_set& m_dependency_list;
  bool m_global_processed; // only one global section per project file is allowed
/*
 * Warning output
 *****************/
  void syntax_warning(const std::string& message,
                      const string_vector& warning_list = string_vector()) const;
/*
 * Lexing
 *********/
  bool next_token(std::string& token,
                  const std::set<char>& special_characters = s_special_characters);
  bool next_list_token(const configuration& configuration,
                       std::string& token);
  bool process_conditional();
  // reads colon-lists of dependencies, ends at first '{'
  void read_dependency_list(dependency_set& dependencies);
/*
 * Parsing
 **********/
  // conditionals
  bool test_condition(const std::function<bool(const std::string&)>& config_contains);
  void process_outer_conditional(const configuration& configuration);      // evaluated against s_configuration, skips full target
  void process_dependency_list_conditional(const configuration& configuration); // evaluated against s_configuration, skips dependenc(y/ies)
  void process_inner_conditional(const configuration& configuration);      // evaluated against m_configuration, skips whole list
  void process_inner_list_conditional(const configuration& configuration); // evaluated against m_configuration, skips item in list
  // item lists
  void parse_file_list(target& target,
                       const file_type type); // matches wildcards to filenames and checks existence
  void parse_source_directory_list(target& target,
                                   const file_type type); // searches m_source_directory+"/"+m_subdirectory
  void parse_build_directory(const file_type type);  // will be created on first use
  void parse_variable_list(string_set& items); // only adds or removes strings from variables
  void parse_library_list(target& target,
                          file_cache& file_cache); // parses -l and -L items, and handle interproject dependencies?
  // main target parser
  void parse_target(target& target, file_cache& file_cache);
  // input validation functions (see Ambrosia wiki for valid input requirements)
  void validate_variable(const std::string& config);
  void validate_filename(const std::string& filename);
  void validate_directory(const std::string& directory);
};

libambrosia_namespace_end

#endif // AMBROSIA_NECTAR_LOADER_H
