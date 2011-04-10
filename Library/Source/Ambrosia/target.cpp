/**
  * target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "target.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

target::target( const string &name, const target_type type,
                const string & text, const size_t line_number )
:   m_parsed( false ),
    m_name( name ),
    m_type( type ),
    m_text( text ),
    m_line_number( line_number )
{   }

libambrosia_namespace_end
