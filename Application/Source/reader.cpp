/**
  * reader.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "reader.h"

// libAmbrosia includes
#include "end_state.h"

// C++ includes
/* <fstream> */
    using std::ifstream;
/* <string> */
    using std::string;

namespace ambrosia
{
    reader::reader( state* parent )
    :   state( parent ),
        m_stream()
    {   }

    state* reader::event()
    {
        const string &project_file( s_build_config.path_to_project_file() );
        m_stream.open( project_file );
        if( !m_stream )
            return new end_state( "Unable to open file: " + project_file, this );

        return new end_state( "reader::reader does little for now.", this );
    }

} // namespace ambrosia
