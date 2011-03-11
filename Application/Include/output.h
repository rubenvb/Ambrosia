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

namespace ambrosia
{
    typedef std::ostream& (*STRFUNC)(std::ostream&);

    class output
    {
    public:
        template<typename T>
        output& operator<<( const T &output )
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
        static std::ostream &s_output_stream;
    };
} // namespace ambrosia

#endif // OUTPUT_H
