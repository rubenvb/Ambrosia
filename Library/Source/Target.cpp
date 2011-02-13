/**
  * Target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class implementation
#include "Target.h"

// C++ includes
/* <string> */
    using std::string;

namespace ambrosia
{
    Target::Target( const size_t line_number, const string &text )
    :   m_line_number( line_number ),
        m_text( text )
    {   }
} // namespace ambrosia
