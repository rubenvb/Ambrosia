/**
  * Ambrosia/Error/nectar_error.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/nectar_error.h"

// linAmbrosia includes
#include "Ambrosia/algorithm.h"
/* "Ambrosia/typedefs.h" */

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

libambrosia_namespace_begin

nectar_error::nectar_error( const string &message, const string &filename,
                            const size_t line_number, const string_vector &list )
:   error( message, list ),
    m_filename( filename ),
    m_line_number( line_number )
{   }

 void nectar_error::output_message() const
{
    cerr << "Error in: " << m_filename << "\n"
         << "line: " << to_string(m_line_number) << "\n";
    error::output_message();
}

libambrosia_namespace_end
