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
#include "Ambrosia/nectar.h"
#include "Ambrosia/status.h"

// C++ includes
/* <fstream> */
    using std::ifstream;
/* <string> */
    using std::string;

namespace ambrosia
{
    reader::reader( state* parent )
    :   state( parent ),
        m_stream(),
        m_targets()
    {   }

    state* reader::event()
    {
        ambrosia::drink_nectar( s_build_config.path_to_project_file(), std::back_inserter(m_targets) );

        if( ambrosia::current_status() == status::error )
            return new end_state( this );

        // parse only targets requested and their dependencies
        // TODO

        return new end_state( "reader::reader does little for now.", this );
    }

} // namespace ambrosia
