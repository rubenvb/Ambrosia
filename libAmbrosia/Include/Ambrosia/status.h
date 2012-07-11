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
 * Ambrosia/error.h
 * libAmbrosia's error handling system. Users of the library functions must
 *  check ambrosia::error::status after every libAmbrosia class function call.
 *  Additionally,
 *
 **/

#ifndef AMBROSIA_STATUS_H
#define AMBROSIA_STATUS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
extern size_t s_error_status_calls;
#endif

enum class status
{
  OK,
  list,
  error
};

// set error status with appropriate messages
void emit_error(const std::string& message);
// directly output warning messages immediately
void emit_warning(const std::string& message);
// syntax error including file and line number
void emit_nectar_error(const std::string& message,
                       const std::string& filename,
                       const size_t line_number);
// nectar warning including file and line number, directly output
void emit_nectar_warning(const std::string& message,
                         const std::string& filename,
                         const size_t line_number);
// store string_set's for later output from emit_error/warning
void emit_error_list(const string_vector& list);
void emit_warning_list(const string_vector& list);
// retrieve current status
bool error_status();
bool error_list_status();
// print messages currently stored and return EXIT_FAILURE
int print_errors();

libambrosia_namespace_end

#endif // AMBROSIA_STATUS_H
