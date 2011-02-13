/**
  * Begin.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Begin.h"

// libAmbrosia includes
#include "Ambrosia/Debug.h"

// Ambrosia includes
#include "Output.h"

// libAmbrosia includes
#include "Ambrosia/Global.h" // for version information

// C++ includes
/* <string> */
    using std::string;

namespace ambrosia
{
    Begin::Begin( const int argc, const char* const argv[], State* parent )
    :   State( parent ),
        m_arguments( string() )
    {
        // Welcome message
        Output() << "Welcome to Ambrosia, version "
                   << app_version_major << "."
                   << app_version_minor << "."
                   << app_version_bugfix << ", using libAmbrosia version "
                   << lib_version_major << "."
                   << lib_version_minor << "."
                   << lib_version_bugfix << ".\n";

        // serialize arguments in a string members
        for( int i=1; i<argc; ++i )
        {
            m_arguments += argv[i];
            m_arguments += " ";
        }
    }

    State* Begin::event()
    {
        Debug() << "Processing commandline arguments.\n";


        return new End( "Begin does little for now.", this );
    }
} // namespace ambrosia
