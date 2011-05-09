/**
  * Ambrosia/status.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "status.h"

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

libambrosia_namespace_begin

// "private" variables
status s_status = status::none;
string s_errors = string();
string s_warnings = string();

bool error_status()
{
    print_warnings();
    return s_status == status::error;
}
bool warning_status()
{
    return s_status >= status::warning;
}

void emit_error( const string &message )
{
    s_errors += "\nError: " + message;
    s_status = status::error;
}
void emit_warning( const string &message )
{
    s_warnings += "\nWarning: " + message + "\n";
    s_status = std::max( s_status, status::warning );
}

void print_errors()
{
    cerr << s_errors << "\n";
    s_errors.clear();
    if( s_warnings.empty() )
        s_status = status::none;
    else
       s_status = status::warning;
}
void print_warnings()
{
    cerr << s_warnings;
    s_warnings.clear();
    if( s_status != status::error )
        s_status = status::none;
}

libambrosia_namespace_end
