/**
  * error.h
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
#include "Ambrosia/Global.h"

// C++ includes
#include <string>

namespace ambrosia
{
    enum class status
    {
        none,
        warning,
        error // perhaps with warning
    };

    // set error status with appropriate messages
    void emit_error( const std::string &message );
    void emit_warning( const std::string &message );
    // retrieve current status
    extern status & current_status();
    // print messages currently stored
    void print_errors();
    void print_warnings();
} // namespace ambrosia

#endif // ERROR_H
