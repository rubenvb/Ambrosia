/**
  * Ambrosia/build_config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "build_config.h"

// libAmbrosia includes
#include "algorithm.h"
#include "enum_maps.h"
#include "debug.h"
#include "status.h"
#include "platform.h"
/* "typedefs.h" */

// C++ includes
#include <algorithm>
/* <map> */
    using std::map;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;

libambrosia_namespace_begin

build_config s_build_config = build_config();

build_config::build_config()
:   m_source_directory(),
    m_project_file(),
    m_target_os( build_os ),
    m_target_architecture( build_architecture ),
    m_target_toolchain( toolchain::GNU ),
    m_gnu_prefix(),
    m_user_options(),
    m_target_config()
{   }

/*
 * Setters
 **********/
void build_config::set_source_directory( const string &source_directory )
{
    m_source_directory = source_directory;
    std::replace( m_source_directory.begin(), m_source_directory.end(), '/', directory_seperator );
}
void build_config::set_project_file( const string &project_file )
{
    m_project_file = project_file;
}

void build_config::set_ambrosia_cross( const std::string &cross )
{
    debug() << "build_config::Checking and setting cross-compilation options through Ambrosia specification.\n";

    // verify format
    if( !wildcard_compare( "*-*-*", cross) )
    {
        emit_error( "Ambrosia cross-compile specification should be of the form \'OS-Architecture-Toolchain\'.\n" );
        return;
    }
    else
        debug() << "build_config::cross has correct format.\n";

    // find relevant parts and complain if somethin's wrong
    const size_t architecture_index = cross.find( "-" ) + 1;
    const size_t toolchain_index = cross.find( "-", architecture_index ) + 1;


    // split up the string
    const string os( cross.substr(0, architecture_index-1) );
    const string architecture( cross.substr(architecture_index, toolchain_index-architecture_index-1) );
    const string toolchain( cross.substr(toolchain_index, string::npos ) );
    debug() << "build_config::cross options specified:\n"
            << "              os = " << os << ".\n"
            << "              architecture = " << architecture << ".\n"
            << "              toolchain = " << toolchain << ".\n";

    // set the appropriate internal options
    const auto os_it = os_map.find( os );
    if( os_it == os_map.end() )
        emit_error( "Specified invalid target OS: " + os );
    else
        m_target_os = (*os_it).second;
    const auto architecture_it = architecture_map.find( architecture );
    if( architecture_it == architecture_map.end() )
        emit_error( "Specified invalid target bitness: " + architecture );
    else
        m_target_architecture = (*architecture_it).second;
    const auto toolchain_it = toolchain_map.find( toolchain );
    if( toolchain_it == toolchain_map.end() )
        emit_error( "Specified invalid target toolchain: " + toolchain );
    else
        m_target_toolchain = (*toolchain_it).second;
}
void build_config::set_gnu_prefix( const std::string & )
{
    // TODO: detect and set common GNU/GCC target triplets
    emit_error( "Cross-compiling through GNU prefix has not been implemented yet." );
}
void build_config::add_target_config( const std::string &target, const string_set &options )
{
    const auto it = m_target_config.find( target );
    if( it == m_target_config.end() )
    {
        m_target_config.insert( {target, options} );
    }
    else
        merge_options( *it, options );
}
void build_config::add_general_config( const string_set &options )
{
    const auto end = m_target_config.end();
    for( auto it = m_target_config.end(); it != end; ++it )
    {
        merge_options( *it, options );
    }
}
void build_config::set_user_option( const std::string &option, const std::string &value )
{
    if( !m_user_options.insert({option, value}).second )
        emit_error( "Option: " + option + " previously specified." );
}

/*
 * Getters
 **********/
const string & build_config::source_directory() const
{
    return m_source_directory;
}
const string & build_config::project_file() const
{
    return m_project_file;
}
const std::string build_config::path_to_project_file() const
{
    return m_source_directory + directory_seperator + m_project_file;
}

/*
 * Private functions
 ********************/
void build_config::merge_options( pair<const string, string_set> &target, const string_set &new_options )
{
    // TODO: redo two-pass algorithm for ultimate performance
    string_set &old_options( target.second );
    string_set merged_options;
    string_set duplicate_options;
    // get merged set
    std::set_union( old_options.begin(), old_options.end(),
                    new_options.begin(), new_options.end(),
                    insert_iterator<string_set>(merged_options, merged_options.begin()) );
    // get duplicates
    std::set_intersection( old_options.begin(), old_options.end(),
                           new_options.begin(), new_options.end(),
                           insert_iterator<string_set>(duplicate_options, duplicate_options.begin()) );
    if( !duplicate_options.empty() )
    {
        emit_warning( "Warning: duplicate configuration options (for target " + target.first + "): " );
        const auto end = duplicate_options.end();
        for( auto it = duplicate_options.begin(); it != end; ++it )
        {
            emit_warning( "Warning: duplicate configuration options: " + *it + "\n" );
        }
    }
    // set new options
    old_options = merged_options;
}

libambrosia_namespace_end
