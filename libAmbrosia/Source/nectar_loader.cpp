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
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/syntax_error.h"
#include "Ambrosia/global.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/project.h"
#include "Ambrosia/target.h"

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
  using std::pair;
#include <vector>
  using std::vector;

libambrosia_namespace_begin

const set<char> s_special_characters = {'(', ')', '{', '}', ':', ','};
const set<char> s_special_characters_newline = {'(', ')', '{', '}', ':', ',', '\n'};

nectar_loader::nectar_loader(const string& full_filename,
                             const string& sub_directory,
                             istream& stream,
                             const dependency_set& list )
: m_filename(full_filename),
  m_subdirectory(sub_directory),
  m_stream(stream),
  m_line_number(1),
  m_dependency_list(list),
  m_global_processed(false),
  p_target() // ooh, a dangerous null pointer!
{
  debug(debug::nectar) << "nectar_loader::nectar_loader::filename is " << full_filename << "\n";
}
nectar_loader::~nectar_loader()
{   }

void nectar_loader::extract_nectar(project& project)
{
  debug(debug::nectar_parser) << "nectar_loader::extract_nectar::Processing file: " << m_filename << ".\n";

  // Remove leading BOM
  skip_BOM(m_stream, m_filename);

  // create global target
  project.m_targets.emplace_back(unique_ptr<target>(new target(m_subdirectory, {}, *project::configuration)));

  string token;
  while(next_token(token))
  {
    debug(debug::parser) << "nectar_loader::extract_nectar::processing token: \'" << token << "\'.\n";
    if("(" == token)
      process_outer_conditional();
    else if("global" == token)
    {
      debug(debug::parser) << "nectar_loader::extract_nectar::global section found at line " << m_line_number << ".\n";
      if(m_global_processed)
        throw syntax_error("Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed.", m_filename, m_line_number);

      m_global_processed = true;
      if(next_token(token) && "{" == token)
      {
        p_target = project.m_targets[0].get();
        parse_target();
      }
      else
        throw syntax_error("\'global\' must be followed by \'{\'.", m_filename, m_line_number);
    }
    else if("app" == token || "lib" == token)
    {
      const target_type type = map_value(target_type_map, token);
      debug(debug::parser) << "nectar_loader::extract_nectar::" << token << " section found at line " << m_line_number << ".\n";
      if(next_token(token))
      {
        if("{" == token)
          throw nectar_error("Syntax error: Expected " + token + " name after " + token + ".", m_filename, m_line_number);
        else
        {
          debug(debug::parser) << "nectar_loader::extract_nectar::Processing " << map_value(target_type_map_inverse, type) << ": " << token << ".\n";
          const string target_name(token);
          dependency_set dependencies(m_dependency_list);
          read_dependency_list(dependencies);

          if(!next_token(token) && "{" == token)
            throw syntax_error("Expected \'{\' after " + map_value(target_type_map_inverse, type) + " target name.", m_filename, m_line_number);

          project.m_targets.emplace_back(new target(m_subdirectory, target_name, type, dependencies, project.m_targets[0]->m_build_config));

          p_target = project.m_targets.back().get();
          parse_target();
        }
      }
    }
    else if("sub" == token)
    {
      debug(debug::parser) << "nectar_loader::extract_nectar::sub section found at line " << m_line_number << ".\n";
      // get name and dependencies of sub target
      if(next_token(token))
      {
        // Search for sub-project file: sourcedir/token/token.nectar.txt
        // 1. check for subdirectory
        const string full_subproject_directory = full_directory_name(project::configuration->source_directory(), full_directory_name(m_subdirectory, token));
        if(!directory_exists(full_subproject_directory))
          throw nectar_error("Directory " + full_subproject_directory + " not found.\n"
                             "Subproject names must be identical to the subproject names.", m_filename, m_line_number );

        string subproject_filename = token + ".nectar.txt";
        string full_subproject_filename = full_directory_name(full_subproject_directory, subproject_filename);
        if(!file_exists(full_subproject_filename))
        {
          //TODO: check what happens here...
          debug(debug::nectar) << "nectar_loader::extract_nectar:Subproject filename is same as subdirectory.\n";
        }
        // Opening project file
        auto stream_ptr = open_ifstream(full_subproject_filename);
        auto& stream = *stream_ptr;
        if(stream)
        {
          debug(debug::nectar) << "nectar_loader::extract_nectar:Opening file " << full_subproject_filename << " succeeded.\n";
          // Get sub target dependencies
          dependency_set dependencies;
          read_dependency_list(dependencies);

          nectar_loader subloader(subproject_filename, full_directory_name(m_subdirectory, token), stream, dependencies);
          subloader.extract_nectar(project);
        }
        else // opening file failed
          throw nectar_error("Error opening subproject file: " + full_subproject_filename + ".",  m_filename, m_line_number);
      }
      else
        throw syntax_error("\'sub\' must be followed by the name of the subproject.", m_filename, m_line_number);
    }
    else
      throw syntax_error("Unexpected token: " + token + ". Expected global, app, lib, or sub.", m_filename, m_line_number);
  }
  debug(debug::nectar_parser) << "nectar_loader::Finished with file: " << m_filename << ".\n";
}

/*
 * Warning output
 *****************/
void nectar_loader::syntax_warning(const string& message,
                                   const string_vector& warning_list ) const
{
  debug(debug::status) << "nectar_loader::syntax_warning::Emitting a syntax warning now.\n";
  cerr << "\nSyntax warning: " + m_filename + "\n" +
          "       line " + to_string(m_line_number) + "\n" +
          "       " + message << "\n";
  std::for_each( warning_list.begin(), warning_list.end(), [](const string& item) { cerr << "\n\t" << item; });
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
  m_stream.get(c);
  if (contains(special_characters, c)) {
    addchar();
    return true;
  }
  // Consume but don't do anything with whitespace
  if (std::isspace(c, m_stream.getloc())) {
    return next_token(token, special_characters);
  }
  switch(c) {
    case '"': {
      //addchar();
      while(m_stream.get(c) && c != '"') {
        // Something about first char being whitespace exception
        if (c == '\n') // exception
        addchar();
      }
      if (!m_stream) // exception
      //addchar();
      return true;
    }
    case '\n': {
      ++m_line_number;
      return next_token(token, special_characters);
    }
    case '#': {
      while(m_stream.get(c) && c != '\n');
      if (!m_stream) // unterminated comment exception
      m_stream.putback('\n');
      return next_token(token, special_characters);
    }
  }
  // Identifier, I *think*
  // Hard to extract the original logic..
  // So I just went with the traditional [a-zA-Z_] regex.
  addchar();
  while(m_stream.get(c) && ((c >= 'a' && c <= '\z') || (c >= 'A' && c <= 'Z') || c == '_'))
    addchar();
  m_stream.putback(c);
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

  while(m_stream.get(c))
  {
    debug(debug::lexer) << "nectar_loader::next_token::line number " << m_line_number << ", character: \'" << output_form(c) << "\', token so far: "
                        << output_form(token) << "\n";
    if(inside_quotes)
    {
      debug(debug::lexer) << "nectar_loader::next_token::Inside quotes.\n";
      if('\"' == c)
        break; // end of token at end of quotes
      else if('\n' == c)
        throw syntax_error("Quoted strings cannot span several lines.", m_filename, m_line_number);
      else if(token.empty() && std::isspace(c, m_stream.getloc()))
        throw syntax_error("Beginning quote must not be followed by a whitespace.", m_filename, m_line_number);
      else
        token.append(1, c);
    }
    else
    {
      if(token.empty())
      {
        if('\n' == c)
          ++m_line_number;

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
        else if(std::isspace(c, m_stream.getloc()))
          continue;
        else if('#' == c)
        { // skip over comments
          debug(debug::lexer) << "nectar_loader::next_token::Skipping over comments.\n";
          string temp;
          std::getline(m_stream, temp);
          m_stream.putback('\n');
        }
        else if('\\' == c)
        {
          string temp;
          std::getline(m_stream, temp);
          ++m_line_number;
        }
        else
          token.append(1, c);
      }
      else if(std::isspace(c, m_stream.getloc()) || contains(special_characters, c))
      { // special characters or whitespace seperate tokens
        debug(debug::lexer) << "nectar_loader::next_token::Detected special character or space.\n";
        m_stream.putback(c);
        break;
      }
      else if('\"' == c)
        throw syntax_error("Beginning quotes must be preceded by a whitespace or a special character.", m_filename, m_line_number);
      else
        token.append(1, c);
    }
  }
  if(!token.empty())
    debug(debug::lexer) << "nectar_loader::next_token:Token extracted: \'" << output_form(token) << "\'\n";

  return !token.empty();
}
bool nectar_loader::next_list_token(string& token)
{
  debug(debug::parser) << "nectar_loader::next_list_token::reading next list item.\n";
  size_t curly_braces_count = 0;
  while(next_token(token, s_special_characters_newline))
  {
    debug(debug::parser) << "nectar_loader::next_list_token::token: " << output_form(token) << ".\n";
    if("\n" == token)
    {
      debug(debug::parser) << "nectar_loader::next_list_token::End of list.\n";
      return false; // list has ended
    }
    else if("(" == token)
      process_inner_list_conditional();
    else if("}" == token)
    {
      if(curly_braces_count > 0)
        --curly_braces_count;
      else
      {
        throw syntax_error("Unexpected closing curly brace.", m_filename, m_line_number);
        return false;
      }
    }
    else // normal list item
      return true;
  }
  if(curly_braces_count > 0)
    throw syntax_error("Unclosed curly braces in list.", m_filename, m_line_number);
  return true;
}

void nectar_loader::read_dependency_list(dependency_set& dependencies)
{
  // copy "parent" dependencies
  dependencies = m_dependency_list;
  debug(debug::parser) << "nectar_loader::read_dependency_list::Reading dependencies.\n";
  bool in_list = false;
  target_type type;
  string token;
  while(next_token(token, s_special_characters_newline))
  {
    debug(debug::parser) << "nectar_loader::read_dependency_list::token: " << output_form(token) << ".\n";
    if("{" == token || "\n" == token)
    {
      debug(debug::parser) << "nectar_loader::read_dependency_list::" << dependencies.size() << " dependencies.\n";
      return;
    }
    else if(!in_list)
    {
      if(":" == token)
      {
        if(next_token(token))
        {
          if("lib"==token)
            type = target_type::lib;
          else if("app" == token)
            type = target_type::app;

          debug(debug::parser) << "nectar_loader::read_dependency_list::Detected " << token << " dependencies.\n";
          in_list = true;
          goto insert_dependency;
        }
        else
          throw syntax_error("Expected list of target names after \':\'.", m_filename, m_line_number);
      }
      else
        throw syntax_error("Expected target body \'{\' or dependency list \':\'.", m_filename, m_line_number);
    }
    else if("," == token)
    {
      insert_dependency:
      if(next_token(token))
      {
        debug(debug::parser) << "nectar_loader::read_dependency_list::Inserting dependency: " << token << ".\n";
        const pair<target_type, string> element = {type, token};
        if(!dependencies.insert(element).second)
        {
          if(!contains(m_dependency_list, element))
            throw syntax_error("Double dependency listed: " + map_value(target_type_map_inverse, type) + " " + token + ".", m_filename, m_line_number);
        }
      }
    }
    else
      in_list = false;
  }
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
            m_filename, m_line_number );
          case conditional_operator::or_op:
            result = result || test_condition(config_contains);
            break;
          case conditional_operator::and_op:
            throw syntax_error( "And operator not implemented yet.",
            m_filename, m_line_number );
          default:
            throw std::logic_error( "nectar_loader::test_condition:Operator " + map_value(conditional_operator_map_inverse, op) + " unexpected." );
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
        throw syntax_error("Expected config item after conditional operator " + map_value(conditional_operator_map_inverse, op) + " unexpected.",
                           m_filename, m_line_number);
      }
      else
        previous_was_operator = true;
    }
    else // "token" is a config string
    {
      debug(debug::conditional) << "nectar_loader::test_condition:Testing config string \'" << token << "\'" << " with operator "
                                << map_value(conditional_operator_map_inverse, op) << ".\n";
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
          throw std::logic_error("nectar_loader::test_condition:Operator " + map_value(conditional_operator_map_inverse, op) + " not expected." );
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

void nectar_loader::process_outer_conditional()
{
  throw nectar_error("Outer conditionals not implemented yet.", m_filename, m_line_number);
}
void nectar_loader::process_dependency_list_conditional()
{
  throw nectar_error("Outer list conditionals not implemented yet.", m_filename, m_line_number);
}

void nectar_loader::process_inner_conditional()
{
  debug(debug::parser) << "nectar_loader::process_inner_conditional::Using target config:\n" << p_target->m_build_config.config() << "\n";
  if(test_condition([this](const string& item){ return contains(p_target->m_build_config.config(), item); }))
    debug(debug::parser) << "nectar_loader::process_inner_conditional::condition returned true, nothing to skip.\n";
  else
  {
    debug(debug::parser) << "nectar_loader::process_inner_conditional::conditional returned false, skipping all relevant parts.\n";
    string token;
    while(next_token(token, s_special_characters_newline))
    {
      if( "\n" == token )
        break; // reached the end
      else
        continue; // ignore anything in the list following a false conditional
    }
  }
}
void nectar_loader::process_inner_list_conditional()
{
  throw nectar_error("Inner list conditionals not implemented yet.", m_filename, m_line_number);
}

void nectar_loader::parse_file_list(const file_type type)
{
  debug(debug::parser) << "nectar_loader::parse_file_list::Parsing " << map_value(file_type_map_inverse, type) << " file list.\n";
  bool empty = true; // a list must not be empty
  string token;

  while(next_list_token(token))
  {
    debug(debug::parser) << "nectar_loader::parse_file_list::adding matches to file " << token << " to the file list.\n";
    empty = false;
    p_target->add_source_file(type, token, m_filename, m_line_number); // errors are assembled in this function
  }
  if(empty)
    throw syntax_error( "A list must not be empty.", m_filename, m_line_number );
}
void nectar_loader::parse_source_directory_list( const file_type type )
{
  debug(debug::parser) << "nectar_loader::parse_source_directory_list::Parsing full list, nonexistent directories are kept in error_list.\n";
  bool empty_list = true; // a list must not be empty
  string token;
  string_vector error_list;
  // gather all list items
  while(next_list_token(token))
  {
    const string subdirectory_name = full_directory_name(m_subdirectory, token);

    empty_list = false;

    if(!p_target->add_source_directory(type, subdirectory_name))
      error_list.push_back("line " + to_string(m_line_number) +": " + token); // add the bad directory to error_list
  }
  if(empty_list)
    throw syntax_error("A source directory list must not be empty", m_filename, m_line_number);
  else if( !error_list.empty() )
    throw nectar_error("Some source directories were not found:\n", m_filename, m_line_number, error_list);
}
void nectar_loader::parse_build_directory(const file_type)
{
  throw nectar_error("Build directory list parsing isn't done yet.", m_filename, m_line_number);
}
void nectar_loader::parse_variable_list(string_set&)
{
  throw nectar_error("Variable list parsing isn't done yet.", m_filename, m_line_number);
}

void nectar_loader::parse_library_list()
{
  string token;
  string_vector error_list;
  while(next_list_token(token))
  {
    // LIBS items must be of the form '-lsomelib' or '-Lsomedirectory'
    if(token.size() <= 2)
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", m_filename, m_line_number);
    else if(!token.compare(0, 2, "-l"))
    {
      token = token.substr(2);
      debug(debug::parser) << "nectar_loader::parse_library_list::Found library name: " << token << ".\n";
      if(p_target->add_library(token))
        throw nectar_error("Library is mentioned twice: " + token, m_filename, m_line_number);
    }
    else if(!token.compare(0, 2, "-L"))
    {
      token = token.substr(2);
      debug(debug::parser) << "nectar_loader::parse_library_list::Found library search directory: " << token << ".\n";
      if(!directory_exists(token))
        error_list.push_back(token);
      if(is_absolute_path(token))
      {
        debug(debug::parser) << "nectar_loader::parse_library_list::Absolure library path detected.\n";
        syntax_warning("Absolute paths in project files should be avoided.");
      }
    }
    else
      throw syntax_error("LIBS items must be of the form \'-lsomelib\'' and/or \'-Lsomedirectory\'", m_filename, m_line_number);
  }
}

void nectar_loader::parse_target()
{
  const target_type type = p_target->m_type;
  const std::string target_name(p_target->name());
  debug(debug::parser) << "nectar_loader::parse_target::Processing named target section: " << target_name << ".\n";
  size_t curly_brace_count = 1; // parsing starts inside curly braces block
  string token;
  bool already_modified_NAME = false;

  while(curly_brace_count > 0 && next_token(token))
  {
    if("}" == token)
      --curly_brace_count;
    else if("{" == token)
      ++curly_brace_count;
    else if("(" == token)
      process_inner_conditional();
    else if("CONFIG" == token)
    {
      debug(debug::parser) << "nectar_loader::parse_target::CONFIG detected.\n";
      parse_variable_list(p_target->m_build_config.config());
    }
    else if ("NAME" == token)
    {
      debug(debug::parser) << "nectar_loader::parse_target::NAME detected.\n";
      if(type == target_type::global)
        throw syntax_error("global target must not have a name", m_filename, m_line_number);

      if(already_modified_NAME)
        syntax_warning("NAME is being modified twice in this target section.");

      already_modified_NAME = true;
      if(next_token(token, s_special_characters_newline))
      {
        if("\n" != token)
        {
          p_target->m_output_name = token;
          continue;
        }
      }
      throw syntax_error("NAME must be followed by the target's output name (without prefix/suffix", m_filename, m_line_number );
    }
    else
    {
      file_type type;
      // is it a list of files or librar(y director)ies?
      if(map_value(file_type_map, token, type))
      {
        debug(debug::parser) << "nectar_loader::parse_target::" << token << " list detected.\n";
        if(type == file_type::library)
          parse_library_list();
        else
          parse_file_list(type);

        debug(debug::parser) << "nectar_loader::parse_target::Succesfully parsed list of files or librar(y director)ies.\n";
      } // or a list of directories
      else if(map_value(directory_type_map, token, type))
      {
        debug(debug::parser) << "nectar_loader::parse_target::" << map_value(file_type_map_inverse, type) << " directory list detected.\n";
        const file_type general_type = get_general_type(type);
        if(general_type == file_type::source || type == file_type::header)
          parse_source_directory_list(type);
        else
          throw nectar_error("Parsing of " + token + " not yet implemented.", m_filename, m_line_number);
      }
      else
        throw syntax_error("Unexpected token: " + token, m_filename, m_line_number);
    }
  }
}

void nectar_loader::validate_variable(const string& config)
{
  if(config.find_first_of("*?/$") != string::npos)
    throw syntax_error("Variable names must not contain the following characters: *?/$", m_filename, m_line_number);
}
void nectar_loader::validate_filename(const string& filename)
{
  const string::size_type index = filename.find_first_of("/?*");
  if(index != string::npos && '/' != filename[index])
  { // there might be wildcards before a directory seperator
    if(filename.find('/',index+1) != string::npos)
      throw syntax_error("Directory seperators '/' are not allowed in directory names.", m_filename, m_line_number);
  }
}
void nectar_loader::validate_directory(const string& directory)
{
  if(!(directory.find_first_of("*?") == string::npos))
    throw syntax_error("Wildcard characters ?* are not allowed in directory names.", m_filename, m_line_number);
}

libambrosia_namespace_end
