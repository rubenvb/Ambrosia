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
 * Ambrosia/lexer.h++
 * Reads strings and makes them into tokens and or multi-token strings.
 *
 **/

#ifndef AMBROSIA_LEXER_H
#define AMBROSIA_LEXER_H

// Global include
#include "Ambrosia/global.h++"

// C++ includes
#include <functional>
#include <istream>
#include <set>
#include <string>

namespace ambrosia
{
namespace lib
{

// Forward declarations
class configuration;

// characters that are read as seperate tokens in Ambrosia .nectar.txt files.
extern const std::set<char> special_characters;
extern const std::set<char> special_characters_newline;
extern const std::set<char> special_characters_conditional;

class lexer
{
public:
  lexer(std::istream& stream,
        const std::string& full_filename,
        const std::size_t line_number = 1,
        const std::size_t column_number = 1,
        std::istream::pos_type stream_position = std::istream::pos_type());

  bool next_token(std::string& token,
                  const std::set<char>& special_characters = ::libambrosia::special_characters);
  void previous_token(); // revert stream position to previous token
  bool next_list_token(const configuration& configuration,
                       std::string& token);

  // conditionals
  bool process_conditional();
  bool test_condition(const std::function<bool(const std::string&)>& config_contains);
  void process_outer_conditional(const configuration& configuration); // skips full target
  void process_dependency_set_conditional(const configuration& configuration); // skips dependenc(y/ies)
  void process_inner_conditional(const configuration& configuration); // skips whole list
  void process_inner_list_conditional(const configuration& configuration); // skips item in list

  std::istream& stream;
  const std::string& filename;
  std::size_t line_number;
  std::size_t column_number;
  // store one-token-back-position
  std::istream::pos_type previous_position;
  std::size_t previous_line_number;
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_LEXER_H
