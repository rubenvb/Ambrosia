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
 * lexer.c++
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/lexer.h++"

// Ambrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/configuration.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/Error/error.h++"
#include "Ambrosia/Error/syntax_error.h++"

// C++ includes
#include <functional>
  using std::function;
#include <istream>
  using std::istream;
#include <set>
  using std::set;
#include <stdexcept>
  using std::logic_error;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

const set<char> special_characters = { '(', ')', '{', '}', ':', ',' };
const set<char> special_characters_newline = { '(', ')', '{', '}', ':', ',', '\n' };
const set<char> special_characters_conditional = { '(', ')', '|', '+', '!' };

lexer::lexer(std::istream& stream,
             const string& filename,
             const size_t line_number,
             const size_t column_number,
             std::istream::pos_type stream_position)
: stream(stream),
  filename(filename),
  line_number(line_number),
  column_number(column_number)
{
  stream.seekg(stream_position);
  if(!stream)
    throw error("Unable to seek to position in file: " + filename);
}

/* replacement written quickly by DeadMG. Look into this.
bool lexer::next_token(string& token,
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
bool lexer::next_token(string& token,
                       const set<char>& special_characters)
{
  // TODO: test the *full* hell out of this function
  // FIXME: ugly as hell, alternatives welcome.
  token.clear();
  bool inside_quotes = false;
  char c;
  // backup current position
  previous_position = stream.tellg();
  previous_line_number = line_number;

  while(stream.get(c))
  {
    debug(debug::lexer, "lexer::next_token::line number ", line_number, ", character: \'", output_form(c), "\', token so far: ", output_form(token), "\n");
    if(inside_quotes)
    {
      debug(debug::lexer, "lexer::next_token::Inside quotes.\n");
      if('\\' == c)
      {
        if(stream.get(c))
          token.append(1, c);
        else
          throw syntax_error("Valid escape character expected after \'\\\'", filename, line_number);
      }
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
          debug(debug::lexer, "lexer::next_token::Detected special character.\n");
          token.append(1, c);
          return true;
        }
        else if('\"' == c)
        {
          debug(debug::lexer, "lexer::next_token::Quote detected.\n");
          inside_quotes = true;
          continue;
        }
        else if(std::isspace(c, stream.getloc()))
          continue;
        else if('#' == c)
        { // skip over comments
          debug(debug::lexer, "lexer::next_token::Skipping over comments.\n");
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
        debug(debug::lexer, "lexer::next_token::Detected special character or space.\n");
        stream.putback(c);
        break;
      }
      //else if('\"' == c)
      //  throw syntax_error("Beginning quotes must be preceded by a whitespace or a special character.", filename, line_number);
      else
        token.append(1, c);
    }
  }
  if(!token.empty())
    debug(debug::lexer, "lexer::next_token:Token extracted: \'", output_form(token), "\'\n");

  return !token.empty();
}
void lexer::previous_token()
{
  debug(debug::lexer, "lexer::previous_token::Resetting input stream position.\n");
  stream.seekg(previous_position);
  line_number = previous_line_number;
}

bool lexer::next_list_token(const configuration& configuration,
                            string& token)
{
  debug(debug::lexer, "lexer::next_list_token::reading next list item.\n");
  size_t curly_braces_count = 0;
  while(next_token(token, special_characters_newline))
  {
    debug(debug::lexer, "lexer::next_list_token::token: ", output_form(token), ".\n");
    if("\n" == token)
    {
      debug(debug::lexer, "lexer::next_list_token::End of list.\n");
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

bool lexer::test_condition(const function<bool(const string&)>& config_contains)
{
  bool result = true;
  bool empty_conditional = true;
  ///
  //  - each set of parenthesis is handled recursively
  //  - logical AND: +
  //  - logical OR:  |
  //  - logical NOT: ! TODO!!!!!!
  //  - two bools: "result" and "current"
  //  - "result" keeps global result, and is modified by "+"
  //  - "current" keeps results for "|" and "!"
  //  - syntax checking for invalid
  ///
  string token;
  bool previous_was_operator = false; // only for |!+
  bool negate = false;
  conditional_operator op = conditional_operator::left_parenthesis;
  while(next_token(token, special_characters_conditional))
  {
    if(token == "(")
    {
      debug(debug::conditional, "lexer::test_condition::Found opening parenthesis \'(\';\n");
      if(previous_was_operator)
      {
        switch(op)
        {
          case conditional_operator::not_op:
            throw syntax_error("Not operator not implemented yet.", filename, line_number);
          case conditional_operator::or_op:
            result = result || test_condition(config_contains);
            break;
          case conditional_operator::and_op:
            throw syntax_error( "And operator not implemented yet.",
            filename, line_number );
          default:
            throw std::logic_error("lexer::test_condition:Operator " + conditional_operator_map_inverse.at(op) + " unexpected.");
        }
      }
      else
        throw syntax_error("Opening parenthesis must be preceded by a conditional operator.", filename, line_number);
    }
    else if(token == ")")
    {
      debug(debug::conditional, "lexer::test_condition:Detected closing parenthesis. Returning ", to_string(result), ".\n");
      if(empty_conditional)
        throw syntax_error("Empty conditional statement.", filename, line_number);
      return result;
    }
    else if(map_value(conditional_operator_map, token, op))
    {
      debug(debug::conditional, "lexer::test_condition::Found conditional operator ", token, ".\n");
      if(op == conditional_operator::not_op)
        negate = !negate;
      else if(previous_was_operator)
      {
        throw syntax_error("Expected config item after conditional operator " + conditional_operator_map_inverse.at(op) + " unexpected.", filename, line_number);
      }
      else
        previous_was_operator = true;
    }
    else // "token" is a config string
    {
      debug(debug::conditional, "lexer::test_condition:Testing config string ", token, " with operator ", conditional_operator_map_inverse.at(op), ".\n");
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
          throw std::logic_error("lexer::test_condition:Operator " + conditional_operator_map_inverse.at(op) + " not expected." );
      }
      debug(debug::conditional, "lexer::test_condition:Current condition state is ", to_string(result), ".\n");
    }
  }
  return result;
}

void lexer::process_outer_conditional(const configuration& configuration)
{
  // processes target and fill
  if(test_condition([&configuration](const string& item)  {return contains(configuration.config_strings, item); }))
    debug(debug::parser, "lexer::process_outer_conditional::condition returned true, nothing to skip.\n");
  else
  {
    debug(debug::parser, "lexer::process_outer_conditional::conditional returned false, skipping whole target.\n");
    string token;
    while(next_token(token))
    {
      if(token == "{")
        break;
    }
    debug(debug::parser, "lexer::process_outer_conditional::Found opening brace of target.\n");

    const size_t begin_line_number = line_number; // store line number for error reporting in case of unmatched braces
    size_t curly_braces = 1;
    while(curly_braces > 0 && next_token(token))
    {
      if(token == "{")
        ++curly_braces;
      else if(token == "}")
        --curly_braces;
      debug(debug::parser, "lexer::process_outer_conditional::Curly brace level: ", curly_braces, ".\n");
    }
    if(curly_braces != 0)
      throw syntax_error("Did not find matching curly braces to match here.", filename, begin_line_number);
  }
  debug(debug::parser, "lexer::process_outer_conditional::Finished with outer conditional.\n");
}
void lexer::process_dependency_set_conditional(const configuration& configuration)
{
  if(test_condition([&configuration](const string& item)  {return contains(configuration.config_strings, item); }))
    debug(debug::parser, "lexer::process_dependency_set_conditional::Condition returned true, nothing to skip.\n");
  else
  {
    string token;
    if(!next_token(token))
      throw syntax_error("Expected dependency name after dependency type.", filename, line_number);
  }
  debug(debug::parser, "lexer::process_dependency_set_conditional::Finished with dependency set conditional.\n");
}

void lexer::process_inner_conditional(const configuration& configuration)
{
  debug(debug::parser, "lexer::process_inner_conditional::Using target config:\n", configuration.config_strings, "\n");
  if(test_condition([&configuration](const string& item){ return contains(configuration.config_strings, item); }))
    debug(debug::parser, "lexer::process_inner_conditional::condition returned true, nothing to skip.\n");
  else
  {
    debug(debug::parser, "lexer::process_inner_conditional::conditional returned false, skipping all relevant parts.\n");
    string token;
    while(next_list_token(configuration, token))
    {
      debug(debug::parser, "lexer::process_inner_conditional::ignoring token: \'", token, "\'.\n");
      // ignore anything in the list following a false conditional
      continue;
    }
  }
}
void lexer::process_inner_list_conditional(const configuration& configuration)
{
  debug(debug::parser, "lexer::process_inner_list_conditional::Using target config:\n", configuration.config_strings, "\n");
  if(test_condition([&configuration](const string& item) { return contains(configuration.config_strings, item); }))
    debug(debug::parser, "lexer::process_inner_list_conditional::condition returned true, nothing to skip.\n");
  else
  {
    debug(debug::parser, "lexer::process_inner_list_conditional::conditional returned false, skipping list item.\n");
    string token;
    if(!next_list_token(configuration, token))
      throw syntax_error("A list must not be empty.", filename, line_number);
  }
}

} // namespace lib

} // namespace ambrosia
