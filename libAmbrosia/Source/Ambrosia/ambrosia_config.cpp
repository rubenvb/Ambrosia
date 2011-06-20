/**
  * Ambrosia/ambrosia_config.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "ambrosia_config.h"

// libAmbrosia includes
#include "algorithm.h"
#include "debug.h"
#include "enum_maps.h"
#include "platform.h"
#include "status.h"

// C++ includes
#include <stdexcept>
    using std::logic_error;
/* <string> */
    using std::string;

libambrosia_namespace_begin

ambrosia_config s_ambrosia_config; // declared in global.h

ambrosia_config::ambrosia_config()
:   m_config(),
    m_source_directory(),
    m_project_file(),
    m_build_directory( "." ),
    m_target_os( build_os ),
    m_target_architecture( build_architecture ),
    m_target_toolchain( toolchain::GNU ),
    m_target_config()
{    }

/*
 * Setters
 **********/
void ambrosia_config::set_source_directory( const string &source_directory )
{
    if( !m_source_directory.empty() )
        throw logic_error( "ambrosia_config::set_source_directory called a second time." );

    if( source_directory != "." )
        m_build_directory = "build";

    m_source_directory = source_directory;
    std::replace( m_source_directory.begin(), m_source_directory.end(), '/', directory_seperator );
}
void ambrosia_config::set_project_file( const string &project_file )
{
    m_project_file = project_file;
}

void ambrosia_config::set_ambrosia_cross( const std::string &cross )
{
    debug(4) << "ambrosia_config::Checking and setting cross-compilation options through Ambrosia specification.\n";

    // verify format
    if( !wildcard_compare( "*-*-*", cross) )
    {
        emit_error( "Ambrosia cross-compile specification should be of the form \'OS-Architecture-Toolchain\'.\n" );
        return;
    }
    else
        debug(4) << "ambrosia_config::cross has correct format.\n";

    // find relevant parts and complain if somethin's wrong
    const size_t architecture_index = cross.find( "-" ) + 1;
    const size_t toolchain_index = cross.find( "-", architecture_index ) + 1;

    // split up the string
    const string os_string( cross.substr(0, architecture_index-1) );
    const string architecture_string( cross.substr(architecture_index, toolchain_index-architecture_index-1) );
    const string toolchain_string( cross.substr(toolchain_index, string::npos ) );
    debug(4) << "ambrosia_config::cross options specified:\n"
             << "              os = " << os_string << ".\n"
             << "              architecture = " << architecture_string << ".\n"
             << "              toolchain = " << toolchain_string << ".\n";

    // set the appropriate internal options
    os new_os = build_os; // shut up uninitialized warning
    if( !map_value(os_map, os_string, new_os) )
        emit_error( "Specified invalid target OS: " + os_string );
    else
        m_target_os = new_os;
    architecture new_architecture = build_architecture; // shut up uninitialized warning
    if( map_value(architecture_map, architecture_string, new_architecture) )
        emit_error( "Specified invalid target bitness: " + architecture_string );
    else
        m_target_architecture = new_architecture;
    toolchain new_toolchain = toolchain::GNU; // shut up uninitialized warning
    if( map_value(toolchain_map, toolchain_string, new_toolchain) )
        emit_error( "Specified invalid target toolchain: " + toolchain_string );
    else
        m_target_toolchain = new_toolchain;
}
void ambrosia_config::set_gnu_prefix( const std::string & )
{
    // TODO: detect and set common GNU/GCC target triplets
    emit_error( "Cross-compiling through GNU prefix has not been implemented yet." );
}
void ambrosia_config::add_target_config( const std::string &target, const string_set &options )
{
    string_set duplicates = merge_sets( m_target_config[target], options );
    if( !duplicates.empty() )
        emit_warning( "" );
}

/*
 * Getters
 **********/
const string & ambrosia_config::source_directory() const
{
    return m_source_directory;
}
const string & ambrosia_config::project_file() const
{
    return m_project_file;
}
const std::string ambrosia_config::path_to_project_file() const
{
    return m_source_directory + directory_seperator + m_project_file;
}
const string & ambrosia_config::build_directory() const
{
    return m_build_directory;
}
const os & ambrosia_config::target_os() const
{
    return m_target_os;
}
const architecture & ambrosia_config::target_architecture() const
{
    return m_target_architecture;
}
const toolchain & ambrosia_config::target_toolchain() const
{
    return m_target_toolchain;
}
const map_string_set_string & ambrosia_config::target_config() const
{
    return m_target_config;
}

libambrosia_namespace_end
