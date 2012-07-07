/**
  * Ambrosia/status.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
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
  std::for_each(error_list.begin(), error_list.end(), []( const string& item) { error_messages += "\n\t" + item; });
}

void emit_warning(const string& message)
{
  cerr << "\nWarning: " + message + "\n";
  std::for_each( warning_list.begin(), warning_list.end(), []( const string& item) { cerr << "\n\t" << item; });
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
  error_list.insert(error_list.end(), list.begin(), list.end());
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
