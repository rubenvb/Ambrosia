/**
  * Ambrosia/status.cpp
  * Function implementations + private error/warning string variable.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/status.h"

// C++ includes
#include <algorithm>
#include <iostream>
    using std::cerr;
#include <stdexcept>
    using std::logic_error;
/* <string> */
    using std::string;

libambrosia_namespace_begin

// "private" variables
bool error = false;
string error_messages = string();
string error_warnings = string();
string_set list = string_set();

bool error_status()
{
    return error;
}

void emit_error( const string &message )
{
    error = true;
    error_messages += "\nError: " + message;
    if( !list.empty() )
        std::for_each( list.begin(), list.end(),
                       []( const string &item)
                       { error_messages += "\n\t" + item; } );
}

void emit_warning( const string &message )
{
    cerr << "\nWarning: " + message + "\n";
    if( !list.empty() )
        std::for_each( list.begin(), list.end(),
                       []( const string &item)
                       { cerr << "\n\t" << item; } );
}

void emit_list( const string_set &list )
{
    if( !list.empty() )
        throw logic_error( "status::emit_list::Internal logic failure. String list is not empty." );
}

void print_errors()
{
    cerr << error_messages << "\n";
    error_messages.clear();
    error = true;
}

libambrosia_namespace_end
