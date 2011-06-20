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
bool error = false;
string error_messages = string();
string error_warnings = string();

bool error_status()
{
    return error;
}

void emit_error( const string &message )
{
    error_messages += "\nError: " + message;
    error = true;
}
void emit_warning( const string &message )
{
    cerr << "\nWarning: " + message + "\n";
}

void print_errors()
{
    cerr << error_messages << "\n";
    error_messages.clear();
    error = true;
}

libambrosia_namespace_end
