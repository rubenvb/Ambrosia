/**
  * builder.cpp
  * Class designed to be used as a temporary output stream.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "builder.h"

// libAmbrosia includes
#include "Ambrosia/build_config.h"

namespace ambrosia
{
    builder::builder( state* parent )
    :   state( parent )
    {   }
} // namespace ambrosia
