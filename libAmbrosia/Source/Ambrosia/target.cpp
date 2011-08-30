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

target::target( const string &name, const target_type type,
                const dependency_list &dependencies, const config_base &config )
:   node( name ),
    m_type( type ),
    m_dependencies( dependencies ),
    m_build_config( config ),
    m_source_directories(),
    m_source_files(),
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
        const file_set matches = s_file_store.match_source_files( filename, m_source_directories[type] );
        if( matches.empty() )
            return emit_nectar_error( "No files matching " + filename + " found.",
                                      nectar_file, line_number ); // error will be handled
    }
    else
    {
        const file_set matches = s_file_store.find_source_file( filename, m_source_directories[type] );
        switch( matches.size() )
        {
            case 0:
               return emit_error("");
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
    s_file_store.match_source_files( filename, m_source_directories[type] );
    emit_error( "target::remove_file::Unimplementented." );
}
bool target::add_source_directory( const file_type type, const string &directory )
{
    s_file_store.add_source_directory( directory );
    if( error_status() )
        return false;

    m_source_directories[type].insert( directory );
    return true;
}
void target::remove_directory( const file_type type, const string &directory )
{
    if( m_source_directories[type].erase(directory) )
        emit_warning_list( {directory} );
}

void target::set_output_name( const std::string &name )
{
    m_output_name = name;
}

/*
 * Private functions
 ********************/


libambrosia_namespace_end
