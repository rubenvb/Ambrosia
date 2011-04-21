/***
  * Ambrosia/debug.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "debug.h"

ambrosia_namespace_begin

// static member initialization
const int s_max_debug_level = 9;
int debug::s_level;

#ifdef AMBROSIA_DEBUG
debug::debug( const int debug_level )
    :   m_output( debug_level <= s_level )
#else // AMBROSIA_DEBUG
debug::debug( const int )
#endif // AMRBOSIA_DEBUG
{}

ambrosia_namespace_end
