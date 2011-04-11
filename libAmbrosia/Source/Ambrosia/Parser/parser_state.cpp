/**
  * parser_state.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Parser/parser_state.h"

// libAmbrosia includes
#include "debug.h"

// C++ includes
#include <istream>
    using std::istream;
/* <set> */
    using std::set;
/* <string> */
    using std::string;

libambrosia_namespace_begin

parser_state::parser_state( istream &stream, const size_t line_number, state* parent )
:   state( parent ),
    m_comment(),
    m_token(),
    m_line_number( line_number ),
    m_stream( stream ),
    m_buffer()
{   }
parser_state::~parser_state()
{   }

libambrosia_namespace_end
