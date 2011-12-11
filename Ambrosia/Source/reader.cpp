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
    using libambrosia::debug;
#include "Ambrosia/nectar.h"
#include "Ambrosia/node.h"
    using libambrosia::node;
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"

// Ambrosia includes
#include "end_state.h"
#include "target_verifier.h"

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
    //debug(0) << "reader::reader created.\n";
}
reader::~reader()
{
    //debug(0) << "reader::Destroyed.\n";
}

state* reader::event()
{
    // load project files
    //debug(1) << "reader:::event::calling extract_nectar with filename: "
    //         << lib::s_ambrosia_config.path_to_project_file() << "\n";
    lib::drink_nectar( lib::s_ambrosia_config.project_file(), m_targets );
    if( lib::error_status() )
        return new end_state( this );

    // delete unrequested targets
    lib::filter_dependency_sort( m_targets );
    if( lib::error_status() )
        return new end_state( this );

    return new target_verifier( m_targets, this );
}

ambrosia_namespace_end
