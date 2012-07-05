/**
  * output.h
  * Class designed to be used as a temporary output stream.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef OUTPUT_H
#define OUTPUT_H

// Global include
#include "global.h"

// C++ includes
#include <iostream>

ambrosia_namespace_begin

class output
{
public:
    template<typename T>
    output& operator<<( const T& output )
    {
        s_output_stream << output;
        return *this;
    }
    // for std::endl and other manipulators
    typedef std::ostream& (*STRFUNC)(std::ostream&);
    output& operator<<( STRFUNC func )
    {
        func(s_output_stream);
        return *this;
    }
private:
    static std::ostream& s_output_stream;
};

ambrosia_namespace_end

#endif // OUTPUT_H
