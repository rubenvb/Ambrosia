/**
  * Ambrosia/project.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/project.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

ambrosia_config project::configuration = ambrosia_config();

project::project( file_cache &file_cache)
:   m_file_cache(file_cache),
    m_subprojects()
{   }

// Static functions
void project::set_internal_option( const string &option, const string &value )
{
    debug(debug::commandline) << "commandline::set_internal_option::Ambrosia internal option: " << option
                              << " with value " << value << " being set.\n";

    if( "cross" == option )
    {
        debug(debug::commandline) << "commandline::set_internal_option::Cross-compiling for "
                                  << value << ".\n";
        lib::project::configuration.set_ambrosia_cross( value );
    }
    #ifdef AMBROSIA_DEBUG
    else if( "d" == option || "debug" == option )
    {
        const uint32_t level = lib::from_string<uint32_t>( value );
        // check validity, partial check on input as well
        if( level > lib::s_max_debug_level )//|| level < 0 )
            lib::emit_error( "Debug level must be a number between 0 and 9." );
        debug(debug::always) << "begin::Setting debug level to " << level << ".\n";
        debug::s_level = static_cast<debug::type>(level);
    }
    #endif // AMBROSIA_DEBUG
    else if( "gnu-prefix" == option )
    {
        debug(debug::commandline) << "commandline::set_internal_option::Cross-compiling with GNU prefix "
                                  << value << ".\n";
        lib::project::configuration.set_gnu_prefix( value );
    }
    else
        lib::emit_error( "Unknown option passed to Ambrosia: \n\t-" + option + "=" + value );
}

// Setters

libambrosia_namespace_end
