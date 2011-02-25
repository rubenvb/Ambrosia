/**
  * Error.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/Error.h"

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

namespace ambrosia
{
    // static member initialization
    Error::status Error::current_status = Error::status::none;
    string Error::errors = string();
    string Error::warnings = string();

    void Error::emit_error( const std::string &message )
    {
        errors += "\nError: " + message;
        current_status = status::error;
    }
    void Error::emit_warning( const std::string &message )
    {
        warnings += "\nWarning: " + message;
        current_status = std::max( current_status, status::warning );
    }

    void Error::print_errors()
    {
        cerr << errors;
        errors.clear();
        if( warnings.empty() )
            current_status = status::none;
        else
           current_status = status::warning;
    }
    void Error::print_warnings()
    {
        cerr << warnings;
        warnings.clear();
        if( current_status != status::error )
            current_status = status::none;
    }
} // namespace ambrosia
