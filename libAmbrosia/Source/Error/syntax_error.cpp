/**
  * Ambrosia/Error/syntax_error.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/syntax_error.h"

// libAmbrosia includes
/* "Ambrosia/typedefs.h" */

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

libambrosia_namespace_begin

syntax_error::syntax_error( const string& message, const string& filename,
                            const size_t line_number, const string_vector& list )
:   nectar_error( message, filename, line_number, list )
{   }

void syntax_error::output_message() const
{
    cerr << "Syntax error:\n";
    nectar_error::output_message();
}

libambrosia_namespace_end
