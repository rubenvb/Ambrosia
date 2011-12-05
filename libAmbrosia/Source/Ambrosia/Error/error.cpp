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
#include "Ambrosia/typedefs.h"

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
error::error( const std::string &message, const file_set &list )
:   m_message( message ),
    m_list( list.size() )
{
    auto dest = m_list.begin();
    for_each(list.begin(), list.end(), [&](const file& item) { *dest++ = item.first; });
}

error::~error()


void error::output_message() const
{
    cerr << m_message;
}

libambrosia_namespace_end
