/**
  * Ambrosia/Error/error.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/error.h"

// libAmbrosia includes
/* "Ambrosia/typedefs.h" */

// C++ includes
#include <iostream>
    using std::cerr;
/* <string> */
    using std::string;

libambrosia_namespace_begin

error::error( const string &message, const string_vector &list )
:   m_message( message ),
    m_list( list )
{   }
error::~error()
{   }

void error::output_message() const
{
    cerr << m_message;
}

libambrosia_namespace_end
