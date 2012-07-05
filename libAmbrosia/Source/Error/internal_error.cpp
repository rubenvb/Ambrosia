/**
  * Ambrosia/Error/error.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/internal_error.h"

// C++ includes
#include <iostream>
    using std::cerr;

libambrosia_namespace_begin

internal_error::internal_error( const std::string& message )
:   error( message )
{   }

void internal_error::output_message() const
{
    cerr << "This is an Ambrosia bug. A nasty, smelly bug. Please send in your project file and"
            "the output below to the Ambrosia project so this can be resolved as soon as possible.\n";
    error::output_message();
}

libambrosia_namespace_end
