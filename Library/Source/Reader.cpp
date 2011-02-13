/**
  * Reader.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Reader.h"

// Ambrosia includes
#include "Ambrosia/Error.h"

// C++ includes
#include <fstream>
    using std::ifstream;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    Reader::Reader()
    :   m_targets( vector<string>() )
    {   }

    void Reader::load_nectar( const std::string &filename )
    {
        ifstream stream( filename.c_str() );
        if( !stream )
            emit_error( "Reader: Unable to open file " + filename );



    }

    void Reader::dependency_sort()
    {

    }
} // namespace ambrosia
