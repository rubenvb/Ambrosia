/**
  * Parser/target_parser.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Parser/target_parser.h"

// libAmbrosia includes
#include "target.h"

// C++ includes
/* <istream> */
    using std::istream;

libambrosia_namespace_begin

target_parser::target_parser( target &t, istream &stream )
:   parser( t.filename(), stream, t.line_number() ),
    m_target( t )
{   }

void target_parser::parse()
{

}

libambrosia_namespace_end
