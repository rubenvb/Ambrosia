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
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

ambrosia_config* project::configuration = NULL;

project::project( ambrosia_config &ambrosia_config, file_cache &file_cache)
:   m_file_cache(file_cache),
    m_targets(),
    m_subprojects()
{
    configuration = &ambrosia_config;
}
project::project( file_cache &file_cache)
:   m_file_cache(file_cache),
    m_targets(),
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
        lib::project::configuration->set_ambrosia_cross( value );
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
        lib::project::configuration->set_gnu_prefix( value );
    }
    else
        lib::emit_error( "Unknown option passed to Ambrosia: \n\t-" + option + "=" + value );
}

// read project file(s)
void project::read_project_files()
{
    // open file
    const string &filename = configuration->project_file();
    auto stream_ptr( open_ifstream(filename) );
    auto &stream = *stream_ptr;
    if( !stream )
        throw error( "Unable to open *.nectar.txt file: " + filename );

    // read targets
    debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
    nectar_loader loader( filename, "", stream );

    loader.extract_nectar( m_targets );
}

libambrosia_namespace_end
