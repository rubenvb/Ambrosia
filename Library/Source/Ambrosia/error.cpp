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
    error::status error::s_status = error::status::none;
    string error::s_errors = string();
    string error::s_warnings = string();

    error::status & error::current_status()
    {
        return s_status;
    }

    void error::emit_error( const std::string &message )
    {
        s_errors += "\nError: " + message;
        s_status = status::error;
    }
    void error::emit_warning( const std::string &message )
    {
        s_warnings += "\nWarning: " + message;
        s_status = std::max( s_status, status::warning );
    }

    void error::print_errors()
    {
        cerr << s_errors;
        s_errors.clear();
        if( s_warnings.empty() )
            s_status = status::none;
        else
           s_status = status::warning;
    }
    void error::print_warnings()
    {
        cerr << s_warnings;
        s_warnings.clear();
        if( s_status != status::error )
            s_status = status::none;
    }
} // namespace ambrosia
