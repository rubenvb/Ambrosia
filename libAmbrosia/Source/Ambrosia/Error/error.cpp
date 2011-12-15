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
#include <algorithm>
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
    std::for_each(list.begin(), list.end(), [=](const file& item) mutable
                                            { *dest++ = item.first; });
}
error::~error()
{   }

void error::output_message() const
{
    cerr << "Error:\n"
            "\t" << m_message << "\n";
    if( !m_list.empty() )
    {
        std::for_each( m_list.begin(), m_list.end(),
                       [](const string &item)
                       {
                           cerr << "\t" << item << "\n";
                       } );
    }
}

libambrosia_namespace_end
