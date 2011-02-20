/***
  * Debug.h
  * Class designed to be used as temporary output stream for debugging purposes.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef DEBUG_H
#define DEBUG_H

// Ambrosia includes
#include "Ambrosia/Global.h"

// C++ includes
#include <iostream>
#include <string>

namespace ambrosia
{
    typedef std::ostream& (*STRFUNC)(std::ostream&);

    class Debug
    {
    public:
        Debug()
        {}

        template<typename T>
        #ifdef AMBROSIA_DEBUG
        Debug& operator<<( const T &output )
        {
            std::cerr << output;
        #else
        Debug& operator<<( const T & )
        {
        #endif // AMBROSIA_DEBUG
            return *this;
        }
        // for std::endl and other manipulators
        typedef std::ostream& (*STRFUNC)(std::ostream&);
        #ifdef AMBROSIA_DEBUG
        Debug& operator<<( STRFUNC func )
        {
            func(std::cerr);
        #else
        Debug& operator<<( STRFUNC )
        {
        #endif // AMBROSIA_DEBUG
            return *this;
        }
    };

} // namespace ambrosia

#endif // DEBUG_H
