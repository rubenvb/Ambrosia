/**
  * target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/target.h"

// Ambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/file_store.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;
/* <vector> */
    using std::vector;

libambrosia_namespace_begin

// Static variable initialization

target::target( const string &subdirectory,
                const dependency_list &dependencies, const ambrosia_config &config )
:   node(subdirectory + "::global"),
    m_type( target_type::global ),
    m_dependencies( dependencies ),
    m_build_config( subdirectory, config ),
    m_source_directories(),
    m_source_files(),
    m_libraries(),
    m_output_name( subdirectory + "::global" )
{   }
target::target( const string &name, const target_type type,
                const dependency_list &dependencies, const build_config &config )
:   node( name ),
    m_type( type ),
    m_dependencies( dependencies ),
    m_build_config( config ),
    m_source_directories(),
    m_source_files(),
    m_libraries(),
    m_output_name( name )
{
    debug(6) << "target::Created " << map_value(target_type_map_inverse, type) << ": "
             << name << ".\n";
}

/*
 * Getters
 **********/
const string & target::name() const
{
    return m_name;
}
target_type target::type() const
{
    return m_type;
}
const dependency_list & target::dependencies() const
{
    return m_dependencies;
}
/*
 * Setters
 **********/
build_config & target::config()
{
    return m_build_config;
}
const build_config & target::config() const
{
    return m_build_config;
}

void target::add_source_file( const file_type type, const string &filename,
                              const string &nectar_file, const size_t line_number )
{
    if( contains(filename, "*?") )
    {
        const file_set matches = s_file_store.match_source_files( filename, m_build_config,
                                                                  m_source_directories[type] );
        if( matches.empty() )
            return emit_nectar_error( "No files matching " + filename + " found.",
                                      nectar_file, line_number ); // error will be handled
    }
    else
    {
        const file_set matches = s_file_store.find_source_file( filename, m_build_config,
                                                                m_source_directories[type] );
        switch( matches.size() )
        {
            case 0:
                return emit_nectar_error( "No matches to file " + filename + " found.", filename, line_number );
            case 1:
                m_source_files[type].insert( *matches.begin() );
                break;
            default:
                string_vector ambiguous;
                std::for_each( matches.begin(), matches.end(),
                               [&ambiguous](const file &f)
                               { ambiguous.push_back(f.first); } );
                return emit_error_list( {ambiguous} );
        }
    }
}
void target::remove_file( const file_type type, const string &filename )
{
    s_file_store.match_source_files( filename, m_build_config,
                                     m_source_directories[type] );
    emit_error( "target::remove_file::Unimplementented." );
}
bool target::add_source_directory( const file_type type, const string &directory )
{
    const string full_subdirectory_name = full_directory_name( m_build_config.source_directory(), directory );

    debug(6) << "target::add_source_directory::Checking if directory " << full_subdirectory_name
             << " exists.\n";
    if( !directory_exists(full_subdirectory_name) )
        return false;

    s_file_store.add_source_directory( full_subdirectory_name );
    m_source_directories[type].insert( directory );
    return true;
}
void target::remove_directory( const file_type type, const string &directory )
{
    if( m_source_directories[type].erase(directory) )
        emit_warning_list( {directory} );
}
bool target::add_library( const string &library )
{
    debug(6) << "target::add_library::Adding library " << library << " to target " << m_name << ".\n";
    //TODO: check if library can be linked
    return !(m_libraries.insert(library).second);
}
void target::remove_library( const string &library )
{
    if( m_libraries.erase(library) )
        emit_warning_list( {library} );
}

void target::set_output_name( const std::string &name )
{
    m_output_name = name;
}

/*
 * Private functions
 ********************/


libambrosia_namespace_end
