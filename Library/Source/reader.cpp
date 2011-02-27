/**
  * reader.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "reader.h"

// Ambrosia includes
#include "Ambrosia/error.h"

// C++ includes
#include <fstream>
    using std::ifstream;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    reader::reader()
    :   m_targets( vector<string>() )
    {   }

    void reader::load_nectar( const std::string &filename )
    {
        ifstream stream( filename.c_str() );
        if( !stream )
            error::emit_error( "Reader: Unable to open file " + filename );



    }

    void reader::dependency_sort()
    {

    }
} // namespace ambrosia
