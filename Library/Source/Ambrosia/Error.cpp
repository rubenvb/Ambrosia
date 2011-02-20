/**
  * Error.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/Error.h"

namespace ambrosia
{
    namespace error
    {
        std::string messages;
    }
    void emit_error( const std::string &message )
    {
        error::messages += "\nError: " + message;
        error::current_status = error::status::error;
    }
    void emit_warning( const std::string &message )
    {
        error::messages += "\nWarning: " + message;
        error::current_status = std::max( error::current_status, error::status::warning );
    }
    void print_messages()
    {}
} // namespace ambrosia
