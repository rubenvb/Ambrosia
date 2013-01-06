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
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/nectar_loader.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/boost_wrapper.h"
#include "Ambrosia/build_element.h"
#include "Ambrosia/configuration.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/syntax_error.h"
#include "Ambrosia/global.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/Targets/binary.h"
#include "Ambrosia/Targets/project.h"

// C++ includes
#include <fstream>
  using std::ifstream;
#include <functional>
  using std::function;
#include <ios>
  using std::noskipws;
#include <iostream>
  using std::cerr;
#include <istream>
  using std::istream;
  using std::istream_iterator;
#include <iterator>
  using std::back_insert_iterator;
#include <locale>
#include <memory>
  using std::unique_ptr;
#include <set>
  using std::set;
#include <stack>
  using std::stack;
#include <stdexcept>
  using std::runtime_error;
#include <string>
  using std::string;
#include <utility>
  using std::tuple;
#include <vector>
  using std::vector;

libambrosia_namespace_begin

const set<char> special_characters =
  list_entries_begin
    entry_begin '(' entry_end
    entry_begin ')' entry_end
    entry_begin '{' entry_end
    entry_begin '}' entry_end
    entry_begin ':' entry_end
    entry_begin ',' entry_end
  entries_end;
const set<char> special_characters_newline =
  list_entries_begin
    entry_begin '(' entry_end
    entry_begin ')' entry_end
    entry_begin '{' entry_end
    entry_begin '}' entry_end
    entry_begin ':' entry_end
    entry_begin ',' entry_end
    entry_begin '\n' entry_end
  entries_end;

nectar_loader::nectar_loader(::libambrosia::project& project,
                             const string& full_filename,
                             const string& sub_directory,
                             istream& stream)
: project(project),
  filename(full_filename),
  subdirectory(sub_directory),
  stream(stream),
  line_number(1),
  global_processed(false)
{
  debug(debug::nectar) << "nectar_loader::nectar_loader::filename is " << full_filename << "\n";
}
nectar_loader::~nectar_loader()
{   }

void nectar_loader::extract_nectar()
{
  debug(debug::nectar_parser) << "nectar_loader::extract_nectar::Processing file: " << filename << ".\n";

  // Remove leading BOM
  skip_BOM(stream, filename);

  // Set build directory
  if(project.configuration.source_directory == ".")
    project.configuration.build_directory = "./build";

  string token;
  while(next_token(token))
  {
    debug(debug::parser) << "nectar_loader::extract_nectar::processing token: \'" << token << "\'.\n";

    if("(" == token)
      process_outer_conditional(project.configuration);
    else if("global" == token)
    {
      debug(debug::parser) << "nectar_loader::extract_nectar::global section found at line " << line_number << ".\n";
      if(global_processed)
        throw syntax_error("Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed.", filename, line_number);

      global_processed = true;
      if(next_token(token) && "{" == token)
        parse_target(project, project.file_cache);
      else
        throw syntax_error("\'global\' must be followed by \'{\'.", filename, line_number);
    }
    else if("sub" == token)
    {
      debug(debug::parser) << "nectar_loader::extract_nectar::\'sub\' section found at line " << line_number << ".\n";
      // get name and dependencies of sub target
      if(next_token(token))
      {
        debug(debug::parser) << "nectar_loader::extract_nectar::Processing subproject \'" << token << "\'.\n";
        // Search for sub-project file: sourcedir/token/token.nectar.txt
        // 1. check for subdirectory
        const string full_subproject_directory = project.configuration.source_directory / subdirectory / token;
        if(!platform::directory_exists(full_subproject_directory))
          throw nectar_error("Directory " + full_subproject_directory + " not found.\n"
                             "Subproject names must be identical to the subproject directory.", filename, line_number );

        string subproject_filename = token + ".nectar.txt";
        string full_subproject_filename = full_subproject_directory / subproject_filename;
        if(!platform::file_exists(full_subproject_filename))
        {
          //TODO: check what happens here...
          debug(debug::nectar) << "nectar_loader::extract_nectar:Subproject filename is same as subdirectory.\n";
        }
        // 2. Open project file
        const auto& stream_ptr = platform::open_ifstream(full_subproject_filename);
        auto& sub_stream = *stream_ptr;
        if(sub_stream)
        {
          debug(debug::parser) << "nectar_loader::extract_nectar:Opening file " << full_subproject_filename << " succeeded.\n";
          // Get sub target dependencies
          dependency_set dependencies;
          read_dependency_set(dependencies);
          for(auto&& dependency : dependencies)
          {
            debug(debug::parser) << "nectar_loader::extract_nectar::Dependency: " << dependency.name << "\n";
          }

          // copy configuration and set proper subdirectory
          configuration subconfiguration = project.configuration;
          debug(debug::config) << "nectar_loader::extract_nectar::Setting source directory of subproject \'" << token << "\' to " << full_subproject_directory << ".\n";
          subconfiguration.source_directory = full_subproject_directory;
          debug(debug::config) << "nectar_loader::extract_nectar::Setting build directory of subproject \'" << token << "\' to " << project.configuration.build_directory / token << ".\n";
          subconfiguration.build_directory = project.configuration.build_directory / token;

          project.targets.emplace_back(new libambrosia::project(project.name + "::" + token, subconfiguration, dependencies));

          nectar_loader subloader(*static_cast<libambrosia::project*>(project.targets.back().get()), full_subproject_filename, subdirectory / token, sub_stream);
          subloader.extract_nectar();
        }
        else
          throw nectar_error("Error opening subproject file: " + full_subproject_filename + ".",  filename, line_number);
      }
      else
        throw syntax_error("\'sub\' must be followed by the name of the subproject.", filename, line_number);
    }
    else if("app" == token || "lib" == token)
    {
      const target_type type = target_type_map.at(token);

      debug(debug::parser) << "nectar_loader::extract_nectar:: " << token << " section found at line " << line_number << ".\n";
      if(next_token(token))
      {
        if("{" == token)
          throw syntax_error("Expected " + token + " name after \'" + token + "\'.", filename, line_number);
        else
        {
          // Store target name
          const string target_name = token;
          // Take parent dependencies
          dependency_set dependencies;
          // Add target's dependencies
          read_dependency_set(dependencies);

          configuration target_configuration = project.configuration;
          debug(debug::config) << "nectar_loader::extract_nectar::Setting build directory for binary \'" << target_name << "\'.\n";
          target_configuration.build_directory = target_configuration.build_directory / target_name;

          project.targets.emplace_back(new binary(target_name, target_configuration, type, dependencies));

          parse_target(*project.targets.back(), project.file_cache);
        }
      }
    }
    else if("dep" == token)
    {
      debug(debug::parser)  << "nectar_loader::extract_nectar::dep section found at line " << line_number << ".\n";
      if(next_token(token))
      {
        target_type type = target_type_map.at(token);
        if(type != target_type::library && type != target_type::application)
          throw syntax_error("Dependencies other than \'lib\' or \'app\' are not yet supported.", filename, line_number);
        if(next_token(token))
        {
          const string name = token;
          debug(debug::parser) << "nectar_loader::extract_nectar::Linking dependency " << name << ".\n";

          // Search for dependency in project's dependency_set
          for(auto dependency : project.dependencies)
          {
            debug(debug::parser) << "nectar_loader::extract_nectar::Project dependency: " << dependency.name << "\n";
          }
          auto result = std::find_if(std::begin(project.dependencies), std::end(project.dependencies),
                                     [&name,&type](const dependency& dep) {return dep.name == name && ((dep.type == type) || (dep.type == target_type::external_dependency)); });
          if(result != std::end(project.dependencies))
          {
            debug(debug::parser) << "nectar_loader::extract_nectar::Found project dependency in parent project: " << target_type_map_inverse.at(type) << " " << name << ".\n";
            continue;
          }
          throw nectar_error("Dependency not found: " + name, filename, line_number);
        }
        else
          throw syntax_error("Expected dependency name after dependency type.", filename, line_number);
      }
      else
        throw syntax_error("Expected dependency type (\'app\' or \'lib\') after \'dep\'.", filename, line_number);
    }
    else
      throw syntax_error("Unexpected token: " + token + ". Expected global, sub, app, lib, dep, or test.", filename, line_number);
  }
  debug(debug::nectar_parser) << "nectar_loader::Finished with file: " << filename << ".\n";
}

/*
 * Warning output
 *****************/
void nectar_loader::syntax_warning(const string& message,
                                   const string_vector& warning_list ) const
{
  debug(debug::status) << "nectar_loader::syntax_warning::Emitting a syntax warning now.\n";
  cerr << "\nSyntax warning: " + filename + "\n" +
          "       line " + to_string(line_number) + "\n" +
          "       " + message << "\n";
  for(auto&& item : warning_list)
  {
    cerr << "\n\t" << item;
  }
}
/*
 * Lexing
 *********/
/* replacement written quickly by DeadMG. Look into this.
bool nectar_loader::next_token(string& token,
                               const set<char>& special_characters)
{
  token.clear();
  char c;
  auto addchar = [&] { token.append(1, c); };
  stream.get(c);
  if (contains(special_characters, c)) {
    addchar();
    return true;
  }
  // Consume but don't do anything with whitespace
  if (std::isspace(c, stream.getloc())) {
    return next_token(token, special_characters);
  }
  switch(c) {
    case '"': {
      //addchar();
      while(stream.get(c) && c != '"') {
        // Something about first char being whitespace exception
        if (c == '\n') // exception
        addchar();
      }
      if (!stream) // exception
      //addchar();
      return true;
    }
    case '\n': {
      ++line_number;
      return next_token(token, special_characters);
    }
    case '#': {
      while(stream.get(c) && c != '\n');
      if (!stream) // unterminated comment exception
      stream.putback('\n');
      return next_token(token, special_characters);
    }
  }
  // Identifier, I *think*
  // Hard to extract the original logic..
  // So I just went with the traditional [a-zA-Z_] regex.
  addchar();
  while(stream.get(c) && ((c >= 'a' && c <= '\z') || (c >= 'A' && c <= 'Z') || c == '_'))
    addchar();
  stream.putback(c);
    return true;
}
*/
bool nectar_loader::next_token(string& token,
                               const set<char>& special_characters)
{
  // TODO: test the *full* hell out of this function
  // FIXME: ugly as hell, alternatives welcome.
  token.clear();
  bool inside_quotes = false;
  char c;

  while(stream.get(c))
  {
    debug(debug::lexer) << "nectar_loader::next_token::line number " << line_number << ", character: \'" << output_form(c) << "\', token so far: "
                        << output_form(token) << "\n";
    if(inside_quotes)
    {
      debug(debug::lexer) << "nectar_loader::next_token::Inside quotes.\n";
      if('\"' == c)
        break; // end of token at end of quotes
      else if('\n' == c)
        throw syntax_error("Quoted strings cannot span several lines.", filename, line_number);
      else if(token.empty() && std::isspace(c, stream.getloc()))
        throw syntax_error("Beginning quote must not be followed by a whitespace.", filename, line_number);
      else
        token.append(1, c);
    }
    else
    {
      if(token.empty())
      {
        if('\n' == c)
          ++line_number;

        if(contains(special_characters, c))
        { // special characters are tokens of their own
          debug(debug::lexer) << "nectar_loader::next_token::Detected special character.\n";
          token.append(1, c);
          return true;
        }
        else if('\"' == c)
        {
          debug(debug::lexer) << "nectar_loader::next_token::Quote detected.\n";
          inside_quotes = true;
          continue;
        }
        else if(std::isspace(c, stream.getloc()))
          continue;
        else if('#' == c)
        { // skip over comments
          debug(debug::lexer) << "nectar_loader::next_token::Skipping over comments.\n";
          string temp;
          std::getline(stream, temp);
          stream.putback('\n');
        }
        else if('\\' == c)
        {
          string temp;
          std::getline(stream, temp);
          ++line_number;
        }
        else
          token.append(1, c);
      }
      else if(std::isspace(c, stream.getloc()) || contains(special_characters, c))
      { // special characters or whitespace seperate tokens
        debug(debug::lexer) << "nectar_loader::next_token::Detected special character or space.\n";
        stream.putback(c);
        break;
      }
      else if('\"' == c)
        throw syntax_error("Beginning quotes must be preceded by a whitespace or a special character.", filename, line_number);
      else
        token.append(1, c);
    }
  }
  if(!token.empty())
    debug(debug::lexer) << "nectar_loader::next_token:Token extracted: \'" << output_form(token) << "\'\n";

  return !token.empty();
}

bool nectar_loader::next_list_token(const configuration& configuration,
                                    string& token)
{
  debug(debug::parser) << "nectar_loader::next_list_token::reading next list item.\n";
  size_t curly_braces_count = 0;
  while(next_token(token, special_characters_newline))
  {
    debug(debug::parser) << "nectar_loader::next_list_token::token: " << output_form(token) << ".\n";
    if("\n" == token)
    {
      debug(debug::parser) << "nectar_loader::next_list_token::End of list.\n";
      return false; // list has ended
    }
    else if("(" == token)
      process_inner_list_conditional(configuration);
    else if("}" == token)
    {
      if(curly_braces_count != 0)
        --curly_braces_count;
      else
      {
        throw syntax_error("Unexpected closing curly brace.", filename, line_number);
        return false;
      }
    }
    else // normal list item
      return true;
  }
  if(curly_braces_count != 0)
    throw syntax_error("Unclosed curly braces in list.", filename, line_number);
  return true;
}

void nectar_loader::read_dependency_set(dependency_set& dependencies)
{
  debug(debug::parser) << "nectar_loader::read_dependency_set::Reading dependencies.\n";
  bool in_list = false;
  target_type type = target_type::library; // dummy to shut up GCC warning
  string token;
  while(next_token(token, special_characters_newline))
  {
    debug(debug::parser) << "nectar_loader::read_dependency_set::token: " << output_form(token) << "\n";
    if("{" == token || "\n" == token)
      break; // finished
    else if(!in_list)
    {
      if(":" == token)
      {
        if(next_token(token) && (token == "lib" || token == "app"))
        {
          type = target_type_map.at(token);
          debug(debug::parser) << "nectar_loader::read_dependency_set::Detected " << token << " dependencies.\n";
          in_list = true;
          goto insert_dependency;
        }
        else
          throw syntax_error("Expected list of target names after \':\'.", filename, line_number);
      }
      else
        throw syntax_error("Expected target body \'{\' or dependency list \':\'.", filename, line_number);
    }
    else if("," == token)
    {
      insert_dependency:
      if(next_token(token))
      {
        if("(" == token)
        {
          process_dependency_set_conditional(project.configuration);

          if(!next_token(token))
            throw syntax_error("Expected dependency name after dependency type", filename, line_number);
          if("{" == token)
            break;
        }
        const string& name = token;
        debug(debug::parser) << "nectar_loader::read_dependency_set::Locating " << target_type_map_inverse.at(type) << " dependency: " << token << ".\n";
        const size_t number_of_dependencies = dependencies.size();
        find_dependencies(project, type, name, std::inserter(dependencies, dependencies.begin()));
        if(number_of_dependencies == dependencies.size())
          throw nectar_error("Dependencies must be specified as \'dep\' targets in relavant project files: " + name, filename, line_number);
      }
    }
    else
      in_list = false;
  }
  debug(debug::parser) << "nectar_loader::read_dependency_set::Finished reading " << dependencies.size() << " dependencies.\n";
}

/*
 * Parsing
 **********/
bool nectar_loader::test_condition(const function<bool(const string&)>& config_contains)
{
  bool result = true;
  bool empty_conditional = true;
  /*
      - each set of parenthesis is handled recursively
      - logical AND: +
      - logical OR:  |
      - logical NOT: ! TODO!!!!!!
      - two bools: "result" and "current"
      - "result" keeps global result, and is modified by "+"
      - "current" keeps results for "|" and "!"
      - syntax checking for invalid
    */
  string token;
  bool previous_was_operator = false; // only for |!+
  bool negate = false;
  conditional_operator op = conditional_operator::left_parenthesis;
  while(next_token(token))
  {
    if(token == "(")
    {
      debug(debug::conditional) << "nectar_loader::test_condition::Found opening parenthesis \'(\';\n";
      if(previous_was_operator)
      {
        switch(op)
        {
          case conditional_operator::not_op:
            throw syntax_error( "Not operator not implemented yet.",
            filename, line_number );
          case conditional_operator::or_op:
            result = result || test_condition(config_contains);
            break;
          case conditional_operator::and_op:
            throw syntax_error( "And operator not implemented yet.",
            filename, line_number );
          default:
            throw std::logic_error( "nectar_loader::test_condition:Operator " + conditional_operator_map_inverse.at(op) + " unexpected." );
        }
      }
    }
    else if(token == ")")
    {
      debug(debug::conditional) << "nectar_loader::test_condition:Detected closing parenthesis. Returning " << to_string(result) << ".\n";
      if(empty_conditional)
        syntax_warning("Empty conditional statement.");
      return result;
    }
    else if(map_value(conditional_operator_map, token, op))
    {
      debug(debug::conditional) << "nectar_loader::test_condition::Found conditional operator " << token << ".\n";
      if(op == conditional_operator::not_op)
        negate = !negate;
      else if(previous_was_operator)
      {
        throw syntax_error("Expected config item after conditional operator " + conditional_operator_map_inverse.at(op) + " unexpected.",
                           filename, line_number);
      }
      else
        previous_was_operator = true;
    }
    else // "token" is a config string
    {
      debug(debug::conditional) << "nectar_loader::test_condition:Testing config string \'" << token << "\'" << " with operator "
                                << conditional_operator_map_inverse.at(op) << ".\n";
      empty_conditional = false;
      switch(op)
      {
        case conditional_operator::left_parenthesis:
          result = config_contains(token);
          break;
        case conditional_operator::or_op:
          result = result || config_contains(token);
          break;
        case conditional_operator::and_op:
          result = result && config_contains(token);
          break;
        default:
          throw std::logic_error("nectar_loader::test_condition:Operator " + conditional_operator_map_inverse.at(op) + " not expected." );
      }
      debug(debug::conditional) << "nectar_loader::test_condition:Current condition state is " << to_string(result) << ".\n";
    }
  }
  // My very own recursive implementation - broken

/*  bool previous_was_operator = true;
    bool current = false; // keep track of current state
    string token;
    while( next_token(token) )
    {
        conditional_operator op;
        if( map_value(conditional_operator_map, token, op) )
        {
            debug(6) << "nectar_loader::test_condition::Operator found: " << token << ".\n";
            if( previous_was_operator )
            {
                debug(7) << "nectar_loader::test_condition::Previous token was operator.\n";
                if( op == conditional_operator::not_op )
                {
                    debug(7) << "nectar_loader::test_condition::Operator \'!\' (logical not) found.\n";
                    current = !current; // negate next
                }
                else
                {
                    emit_syntax_error( "Conditional operators \'+\', \'|\', \')\', and \'(\' must be followed by a CONFIG string." );
                    break;
                }
            }
            else
            {
                switch( op )
                {
                    case conditional_operator::right_parenthesis:
                        debug(7) << "nectar_loader::test_condition::Right parenthesis \')\' found.\n";
                        // TODO: allow conditionals without outer parenthesis of the form (a+b)|(c)
                        return result;
                    case conditional_operator::left_parenthesis: // recurse
                        return test_condition( config_contains );
                    case conditional_operator::plus_op:
                        result = result && current; // "current" -> "result"
                        if( !result ) // negative when an element of a "+" expression is false
                            current = false; // reset "current"
                        break;
                    case conditional_operator::or_op: // combine with "current"
                        throw runtime_error( "TODO" );
                    case conditional_operator::not_op: // unreachable
                        throw runtime_error( "Internal logic error in nectar_loader::resolve_conditional" );
                }
            }
            previous_was_operator = true;
        }
        else if( !previous_was_operator )
        {
            emit_syntax_error( "In a conditional all CONFIG strings must be seperated by a conditional operator \'+\', \'|\', \')\', or \'(\'." );
            break;
        }
        else // previous was operator, so now we have a CONFIG string
        {
            // check CONFIG string, and perform a logical OR
            // TODO: check effect of "!"
            current = current || config_contains(token);
            previous_was_operator = false;
        }
    }*/
  return result;
}

void nectar_loader::process_outer_conditional(const configuration& configuration)
{
  // processes target and fill
  if(test_condition([&configuration](const string& item)  {return contains(configuration.config_strings, item); }))
    debug(debug::parser) << "nectar_loader::process_outer_conditional::condition returned true, nothing to skip.\n";
  else
  {
    debug(debug::parser) << "nectar_loader::process_outer_conditional::conditional returned false, skipping whole target.\n";
    string token;
    while(next_token(token))
    {
      if(token == "{")
        break;
    }
    debug(debug::parser) << "nectar_loader::process_outer_conditional::Found opening brace of target.\n";

    const size_t begin_line_number = line_number; // store line number for error reporting in case of unmatched braces
    size_t curly_braces = 1;
    while(curly_braces > 0 && next_token(token))
    {
      if(token == "{")
        ++curly_braces;
      else if(token == "}")
        --curly_braces;
      debug(debug::parser) << "nectar_loader::process_outer_conditional::Curly brace level: " << curly_braces << ".\n";
    }
    if(curly_braces != 0)
      throw syntax_error("Did not find matching curly braces to match here.", filename, begin_line_number);
  }
  debug(debug::parser) << "nectar_loader::process_outer_conditional::Finished with outer conditional.\n";
}
void nectar_loader::process_dependency_set_conditional(const configuration& configuration)
{
  if(test_condition([&configuration](const string& item)  {return contains(configuration.config_strings, item); }))
    debug(debug::parser) << "nectar_loader::process_dependency_set_conditional::Condition returned true, nothing to skip.\n";
  else
  {
    string token;
    if(!next_token(token))
      throw syntax_error("Expected dependency name after dependency type.", filename, line_number);
  }
  debug(debug::parser) << "nectar_loader::process_dependency_set_conditional::Finished with dependency set conditional.\n";
}

void nectar_loader::process_inner_conditional(const configuration& configuration)
{
  debug(debug::parser) << "nectar_loader::process_inner_conditional::Using target config:\n" << configuration.config_strings << "\n";
  if(test_condition([&configuration](const string& item){ return contains(configuration.config_strings, item); }))
    debug(debug::parser) << "nectar_loader::process_inner_conditional::condition returned true, nothing to skip.\n";
  else
  {
    debug(debug::parser) << "nectar_loader::process_inner_conditional::conditional returned false, skipping all relevant parts.\n";
    string token;
    while(next_token(token, special_characters_newline))
    {
      if( "\n" == token )
        break; // reached the end
      else
        continue; // ignore anything in the list following a false conditional
    }
  }
}
void nectar_loader::process_inner_list_conditional(const configuration& /*configuration*/)
{
  throw nectar_error("Inner list conditionals not implemented yet.", filename, line_number);
}

void nectar_loader::parse_file_list(target& target,
                                    const file_type type,
                                    file_cache& file_cache)
{
  debug(debug::parser) << "nectar_loader::parse_file_list::Parsing " << file_type_map_inverse.at(type) << " file list.\n";
  bool empty = true; // a list must not be empty
  string token;

  while(next_list_token(target.configuration, token))
  {
    debug(debug::parser) << "nectar_loader::parse_file_list::adding matches to file " << token << " to the file list.\n";
    empty = false;
    target.add_source_file(type, token, file_cache, filename, line_number); // errors are assembled in this function
  }
  if(empty)
    throw syntax_error("A list must not be empty.", filename, line_number);
}
void nectar_loader::parse_source_directory_list(target& target,
                                                const file_type type,
                                                file_cache& file_cache)
{
  debug(debug::parser) << "nectar_loader::parse_source_directory_list::Parsing full list, nonexistent directories are kept in error_list.\n";
  bool empty_list = true; // a list must not be empty
  string token;
  string_vector error_list;
  // gather all list items
  while(next_list_token(target.configuration, token))
  {
    empty_list = false;

    if(!target.add_source_directory(type, token, file_cache))
      error_list.push_back("line " + to_string(line_number) +": " + token); // add the bad directory to error_list
  }
  if(empty_list)
    throw syntax_error("A source directory list must not be empty", filename, line_number);
  else if(!error_list.empty())
    throw nectar_error("Some source directories were not found:", filename, line_number, error_list);
}
void nectar_loader::parse_build_directory(const file_type)
{
  throw nectar_error("Build directory list parsing isn't done yet.", filename, line_number);
}
void nectar_loader::parse_variable_list(string_set&)
{
  throw nectar_error("Variable list parsing isn't done yet.", filename, line_number);
}

void nectar_loader::parse_library_list(target& target,
                                       file_cache& /*file_cache*/)
{
  string token;
  string_vector error_list;
  while(next_list_token(target.configuration, token))
  {
    // LIBS items must be of the form '-lsomelib' or '-Lsomedirectory'
    if(token.size() <= 2)
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", filename, line_number);
    else if(!token.compare(0, 2, "-l"))
    {
      token = token.substr(2);
      debug(debug::parser) << "nectar_loader::parse_library_list::Found library name: " << token << ".\n";
      target.add_library(token, filename, line_number);
    }
    else if(!token.compare(0, 2, "-L"))
    {
      token = token.substr(2);
      debug(debug::parser) << "nectar_loader::parse_library_list::Found library search directory: " << token << ".\n";
      if(!platform::directory_exists(token))
        error_list.push_back(token);
      if(platform::is_absolute_path(token))
      {
        debug(debug::parser) << "nectar_loader::parse_library_list::Absolute library path detected.\n";
        syntax_warning("Absolute paths in project files should be avoided.");
      }
    }
    else
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", filename, line_number);
  }
}

void nectar_loader::parse_target(target& target,
                                 file_cache& file_cache)
{
  const std::string target_name(target.name);
  debug(debug::parser) << "nectar_loader::parse_target::Processing named target section: \'" << target_name << "\'.\n";
  size_t curly_brace_count = 1; // parsing starts inside curly braces block
  string token;
  bool already_modified_NAME = false;

  while(curly_brace_count > 0 && next_token(token))
  {
    debug(debug::parser) << "nectar_loader::parse_target::Token: \'" << token << "\'.\n";
    if("}" == token)
      --curly_brace_count;
    else if("{" == token)
      ++curly_brace_count;
    else if("(" == token)
      process_inner_conditional(target.configuration);
    else if("CONFIG" == token)
    {
      debug(debug::parser) << "nectar_loader::parse_target::CONFIG detected.\n";
      parse_variable_list(target.configuration.config_strings);
    }
    else if ("NAME" == token)
    {
      debug(debug::parser) << "nectar_loader::parse_target::NAME detected.\n";
      //if(type == target_type::global)
      //  throw syntax_error("global target must not have a name", filename, line_number);

      if(already_modified_NAME)
        syntax_warning("NAME is being modified twice in this target section.");

      already_modified_NAME = true;
      if(next_token(token, special_characters_newline))
      {
        if("\n" != token)
        {
          target.configuration.name = token;
          continue;
        }
      }
      throw syntax_error("NAME must be followed by the target's output name (without prefix/suffix", filename, line_number );
    }
    else
    {
      file_type type;
      // is it a list of files or librar(y director)ies?
      if(map_value(file_type_map, token, type))
      {
        debug(debug::parser) << "nectar_loader::parse_target::" << token << " list detected.\n";
        if(type == file_type::library)
          parse_library_list(target, file_cache);
        else
          parse_file_list(target, type, file_cache);

        debug(debug::parser) << "nectar_loader::parse_target::Succesfully parsed list of files or librar(y director)ies.\n";
      } // or a list of directories
      else if(map_value(directory_type_map, token, type))
      {
        debug(debug::parser) << "nectar_loader::parse_target::" << file_type_map_inverse.at(type) << " directory list detected.\n";
        const file_type general_type = get_general_type(type);
        if(general_type == file_type::source || type == file_type::header)
          parse_source_directory_list(target, type, file_cache);
        else
          throw nectar_error("Parsing of " + token + " not yet implemented.", filename, line_number);
      }
      else
        throw syntax_error("Unexpected token: " + token, filename, line_number);
    }
  }
}

void nectar_loader::validate_variable(const string& config)
{
  if(config.find_first_of("*?/$") != string::npos)
    throw syntax_error("Variable names must not contain the following characters: *?/$", filename, line_number);
}
void nectar_loader::validate_filename(const string& filename)
{
  const string::size_type index = filename.find_first_of("/?*");
  if(index != string::npos && '/' != filename[index])
  { // there might be wildcards before a directory seperator
    if(filename.find('/',index+1) != string::npos)
      throw syntax_error("Directory seperators '/' are not allowed in directory names.", filename, line_number);
  }
}
void nectar_loader::validate_directory(const string& directory)
{
  if(!(directory.find_first_of("*?") == string::npos))
    throw syntax_error("Wildcard characters ?* are not allowed in directory names.", filename, line_number);
}

libambrosia_namespace_end
