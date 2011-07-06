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
#include "Ambrosia/Configuration/build_config.h"
#include "Ambrosia/debug.h"

ambrosia_namespace_begin

builder::builder( state* parent )
:   state( parent )
{
    debug(0) << "builder::Builder state created.\n";
}
builder::~builder()
{
    debug(0) << "builder::Destroyed.\n";
}

ambrosia_namespace_end
