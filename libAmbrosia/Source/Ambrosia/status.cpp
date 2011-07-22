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
bool warning = false;
string error_messages = string();
string warning_messages = string();
string_vector error_list{};
string_vector warning_list{};

bool error_status()
{
    return error;
}

void emit_error( const string &message )
{
    error = true;
    error_messages += "\nError: " + message;
    std::for_each( error_list.begin(), error_list.end(),
                   []( const string &item)
                   { error_messages += "\n\t" + item; } );
}

void emit_warning( const string &message )
{
    cerr << "\nWarning: " + message + "\n";
    std::for_each( warning_list.begin(), warning_list.end(),
                   []( const string &item)
                   { cerr << "\n\t" << item; } );
}

void emit_error_list( const string_vector &list )
{
//    if( !error_list.empty() )
//        throw logic_error( "status::emit_error_list::Internal logic failure. String list is not empty." );

    error_list.insert( error_list.end(), list.begin(), list.end() );
}
void emit_warning_list( const string_vector &list )
{
    if( !warning_list.empty() )
        throw logic_error( "status::emit_warning_list::Internal logic failure. String list is not empty." );

    warning_list = list;
}

void print_errors()
{
    cerr << error_messages << "\n";
    error_messages.clear();
    error = true;
}

libambrosia_namespace_end
