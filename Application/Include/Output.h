/**
  * Output.h
  * Class designed to be used as a temporary output stream.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef OUTPUT_H
#define OUTPUT_H

// C++ includes
#include <iostream>

namespace ambrosia
{
    typedef std::ostream& (*STRFUNC)(std::ostream&);

    class Output
    {
    public:
        //Output();

        template<typename T>
        Output& operator<<( const T &output )
        {
            m_output_stream << output;
            return *this;
        }
        // for std::endl and other manipulators
        typedef std::ostream& (*STRFUNC)(std::ostream&);
        Output& operator<<( STRFUNC func )
        {
            func(m_output_stream);
            return *this;
        }
    private:
        static std::ostream &m_output_stream;
    };
    std::ostream &Output::m_output_stream = std::cout;
} // namespace ambrosia


#endif // OUTPUT_H
