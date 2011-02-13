/**
  * Error.h
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
    namespace error
    {
        enum class status
        {
            none,
            warning,
            error // perhaps with warning
        };
        status current_status;
    } // namespace error
    void emit_error( const std::string &message );
    void emit_warning( const std::string &message );
    void print_messages();
} // namespace ambrosia

#endif // ERROR_H
