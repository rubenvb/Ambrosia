/**
  * Ambrosia/error.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/error.h"

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

namespace ambrosia
{
    // static member initialization
    error::status error::current_status = error::status::none;
    string error::errors = string();
    string error::warnings = string();

    void error::emit_error( const std::string &message )
    {
        errors += "\nError: " + message;
        current_status = status::error;
    }
    void error::emit_warning( const std::string &message )
    {
        warnings += "\nWarning: " + message;
        current_status = std::max( current_status, status::warning );
    }

    void error::print_errors()
    {
        cerr << errors;
        errors.clear();
        if( warnings.empty() )
            current_status = status::none;
        else
           current_status = status::warning;
    }
    void error::print_warnings()
    {
        cerr << warnings;
        warnings.clear();
        if( current_status != status::error )
            current_status = status::none;
    }
} // namespace ambrosia
