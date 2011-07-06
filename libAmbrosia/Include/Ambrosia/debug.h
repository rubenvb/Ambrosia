/***
  * Ambrosia/debug.h
  * Class designed to be used as temporary output stream for debugging purposes.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef DEBUG_H
#define DEBUG_H

// Ambrosia includes
#include "Ambrosia/global.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iostream>

ambrosia_namespace_begin

extern const int s_max_debug_level;

class debug
{
public:
    typedef std::ostream& (*stream_function)(std::ostream&);

    static int s_level;

    debug( const int debug_level = s_level );

    template<typename T>
    #ifdef AMBROSIA_DEBUG
    debug& operator<<( const T &output )
    {
        if( m_output )
            std::cerr << output;
    #else
    debug& operator<<( const T & )
    {
    #endif // AMBROSIA_DEBUG
        return *this;
    }
    // for std::endl and other manipulators
    #ifdef AMBROSIA_DEBUG
    debug& operator<<( stream_function func )
    {
        if( m_output )
            func( std::cerr );
    #else // AMBROSIA_DEBUG
    debug& operator<<( stream_function )
    {
    #endif // AMBROSIA_DEBUG
        return *this;
    }

private:
    #ifdef AMBROSIA_DEBUG
    const bool m_output;
    #endif // AMBROSIA_DEBUG
};

template<>
debug& debug::operator<<( const string_set & );

ambrosia_namespace_end

#endif // DEBUG_H
