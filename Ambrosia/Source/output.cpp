/**
  * output.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "output.h"

// C++ includes
/* <iostream> */
    using std::cout;
    using std::ostream;

ambrosia_namespace_begin

// static member initialization
ostream& output::s_output_stream = cout;

ambrosia_namespace_end
