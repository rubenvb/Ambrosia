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
                const dependency_list &dependencies, const build_config &config )
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
void target::add_config( const string &config )
{
    if( !m_build_config.add_config(config) )
        emit_error_list( {config} ); // value already present
}
void target::remove_config( const string &config )
{
    if( !m_build_config.remove_config(config) )
        emit_warning_list( {config} );
}

void target::add_file( const file_type type, const string &filename )
{
    if( contains(filename, "*?") )
    {
        const file_set matches = s_file_store.match_source_files( filename, m_source_directories[type] );
        if( matches.empty() )
            return emit_error(); // error will be handled
    }
    else
    {
        const file_set matches = s_file_store.find_source_file( filename, m_source_directories[type] );
        switch( matches.size() )
        {
            case 0:
                return emit_error();
            case 1:
                m_source_files[type].insert( *matches.begin() );
            default:
            return emit_error_list();
        }
    }
}
void target::remove_file( const file_type type, const string &filename )
{
    s_file_store.match_source_files( filename, m_source_directories[type] );
    return {"unimplemented"};
}
void target::add_directory( const file_type type, const string &directory )
{


    if( !m_source_directories[type].insert(directory).second )
        return string_set();
    else
        return { directory };
}
void target::remove_directory( const file_type type, const string &directory )
{
    if( m_source_directories[type].erase(directory) )
        return string_set();
    else
        return { directory };
}

void target::set_output_name( const std::string &name )
{
    m_output_name = name;
}

/*
 * Private functions
 ********************/


libambrosia_namespace_end
