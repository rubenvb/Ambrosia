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
 * Ambrosia/status.cpp
 * Function implementations + private error/warning string variable.
 *
 **/

// Function include
#include "Ambrosia/status.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"

// C++ includes
#include <algorithm>
#include <iostream>
  using std::cerr;
#include <stdexcept>
  using std::logic_error;
#include <string>
  using std::string;

libambrosia_namespace_begin

// "private" variables
status current_status = status::OK;
string error_messages = string();
string warning_messages = string();
string_vector error_list{};
string_vector warning_list{};
#ifdef AMBROSIA_DEBUG
size_t s_error_status_calls = 0;
#endif

bool error_status()
{
#ifdef AMBROSIA_DEBUG
  debug(debug::status) << "status::error_status::Status=" << ((current_status==status::error)?"ERROR":"OK") << ".\n";
  ++s_error_status_calls;
#endif
  return current_status == status::error;
}
bool error_list_status()
{
#ifdef AMBROSIA_DEBUG
  debug(debug::status) << "status::error_list_status::Checking error status now.\n";
  ++s_error_status_calls;
#endif
  return current_status == status::list;
}

void emit_error(const string& message)
{
  debug(debug::status) << "status::emit_error:Emitting error here:\n\t" << message << "\n";
  current_status = status::error;
  error_messages += "\nError: " + message;
  std::for_each(std::begin(error_list), std::end(error_list), []( const string& item) { error_messages += "\n\t" + item; });
}

void emit_warning(const string& message)
{
  cerr << "\nWarning: " + message + "\n";
  std::for_each(std::begin(warning_list), std::end(warning_list), []( const string& item) { cerr << "\n\t" << item; });
}
void emit_nectar_error(const string& message,
                       const string& filename,
                       const size_t line_number)
{
  debug(debug::status) << "status::emit_nectar_error::Emitting a nectar error now.\n";
  emit_error("Syntax error in: " + filename + "\n"
           + "       line: " + to_string(line_number) + "\n"
           + "       " + message);
}
void emit_nectar_warning(const string& message,
                         const string& filename,
                         const size_t line_number)
{
  debug(debug::status) << "status::emit_nectar_warning::Emitting a syntax warning now.\n";
  emit_warning("Syntax warning: " + filename + "\n" +
               "       line " + to_string(line_number) + "\n" +
               "       " + message);
}
void emit_error_list(const string_vector& list)
{
  current_status = status::list;
  error_list.insert(std::begin(error_list), std::begin(list), std::end(list));
}
void emit_warning_list( const string_vector& list )
{
  if(!warning_list.empty())
    throw logic_error("status::emit_warning_list::Internal logic failure. String list is not empty.");

  warning_list = list;
}

int print_errors()
{
  cerr << error_messages << "\n";
  error_messages.clear();
  error_list.clear();
  current_status = status::OK;
  return EXIT_FAILURE;
}

libambrosia_namespace_end
