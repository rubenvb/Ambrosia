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
    void emit_error( const std::string &message );
    void emit_warning( const std::string &message );
    void print_messages();
} // namespace ambrosia
