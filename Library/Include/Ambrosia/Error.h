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
    class Error
    {
    public:
        enum class status
        {
            none,
            warning,
            error // perhaps with warning
        };

        static void emit_error( const std::string &message );
        static void emit_warning( const std::string &message );
        static void print_errors();
        static void print_warnings();

    private:
        Error();
        static status current_status;
        static std::string errors;
        static std::string warnings;
    };
} // namespace ambrosia

#endif // ERROR_H
