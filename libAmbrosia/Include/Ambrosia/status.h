/**
  * Ambrosia/error.h
  * libAmbrosia's error handling system. Users of the library functions must
  *  check ambrosia::error::status after every libAmbrosia class function call.
  *  Additionally,
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ERROR_H
#define ERROR_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "enums.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

// set error status with appropriate messages
void emit_error( const std::string &message );
void emit_warning( const std::string &message );
// retrieve current status
extern bool error_status(); // also prints warnings (less code)
extern bool warning_status();
// print messages currently stored
void print_errors();
void print_warnings();

libambrosia_namespace_end

#endif // ERROR_H
