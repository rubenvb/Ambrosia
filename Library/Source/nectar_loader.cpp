/**
  * reader.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "nectar_loader.h"

// Ambrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/error.h"

// C++ includes
/* <fstream> */
    using std::ifstream;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    nectar_loader::nectar_loader( const string &filename )
    :   m_fstream( filename )
    {
        if( !m_fstream )
            error::emit_error( "Failed to open file: " + filename );
    }

    template<class output_iterator>
    void load_nectar( const std::string &filename, output_iterator &it )
    {
        ifstream stream( filename );
        if( !stream )
            error::emit_error( "Unable to open nectar file " + filename );

        bool global_processed = false;
        string current;
        while( stream >> current )
        {
            if( "global" == current )
            {
                debug() << "nectar::global target found.\n";
            }
        }



    }
} // namespace ambrosia
