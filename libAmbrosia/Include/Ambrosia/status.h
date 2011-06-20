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
// directly output warning messages immediately
void emit_warning( const std::string &message );
// retrieve current status
bool error_status();
// print messages currently stored
void print_errors();

libambrosia_namespace_end

#endif // ERROR_H
