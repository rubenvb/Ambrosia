/**
  * Ambrosia/Error/soft_error.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/soft_error.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

soft_error::soft_error( const string &message )
:   error( message )
{   }
soft_error::~soft_error()
{   }

libambrosia_namespace_end
