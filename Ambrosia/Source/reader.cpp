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
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/node.h"
    using libambrosia::node;
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"

// Ambrosia includes
#include "end_state.h"
    using libambrosia::state;

// C++ includes
/* <fstream> */
    using std::ifstream;
/* <string> */
    using std::string;

ambrosia_namespace_begin

reader::reader( state* parent )
:   state( parent ),
    m_stream(),
    m_targets()
{
    debug(0) << "reader::reader created.\n";
}
reader::~reader()
{
    debug(0) << "reader destroyed.\n";
}

state* reader::event()
{
    // load project files
    lib::drink_nectar( lib::s_ambrosia_config.path_to_project_file(), m_targets );
    if( lib::error_status() )
        return new end_state( this );

    // delete unrequested targets
    lib::filter_dependency_sort( m_targets );
    if( lib::error_status() )
        return new end_state( this );

    return new end_state( "reader::reader does little for now.", this );
}

ambrosia_namespace_end
