/**
  * Ambrosia/debug.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/debug.h"

// C++ includes
#include <algorithm>

ambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
// static member initialization
const int s_max_debug_level = 9;
int debug::s_level = s_max_debug_level;
debug::debug( const int debug_level )
    :   m_output( debug_level <= s_level )
#else // AMBROSIA_DEBUG
debug::debug( const int )
#endif // AMRBOSIA_DEBUG
{}

// specialization for string_set
template<>
#ifdef AMBROSIA_DEBUG
debug& debug::operator<<( const string_set & strings )
{
    if( m_output )
        std::for_each( strings.begin(), strings.end(),
                       [strings](const std::string &item)
                       { std::cerr << "\t" << item << "\n"; } );
#else // AMBROSIA_DEBUG
debug& debug::operator<<( const string_set & )
{
#endif // AMBROSIA_DEBUG
    return *this;
}


ambrosia_namespace_end
