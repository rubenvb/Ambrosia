/**
  * target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "target.h"

// Ambrosia includes
#include "algorithm.h"
#include "ambrosia_config.h"
#include "debug.h"
#include "enum_maps.h"
#include "file_store.h"
#include "platform.h"
#include "status.h"

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
const string_set target::add_config( const string &config )
{
    if( m_build_config.add_config(config) )
        return string_set(); // empty return value is success
    else
        return { config }; // value already present
}
const string_set target::remove_config( const string &config )
{
    if( m_build_config.remove_config(config) )
        return string_set(); // empty retun value is success
    else
        return { config };
}

const string_set target::add_file( const file_type type, const string &filename )
{
    s_file_store.match_source_files( m_source_directories[type], replace_directory_seperators(filename) );
    return {"unimplemented"};
}
const string_set target::remove_file( const file_type type, const string &filename )
{
    s_file_store.match_source_files( m_source_directories[type], replace_directory_seperators(filename) );
    return {"unimplemented"};
}
const string_set target::add_directory( const file_type type, const string &directory )
{
    if( m_source_directories[type].insert(replace_directory_seperators(directory)).second )
        return string_set();
    else
        return { replace_directory_seperators(directory) };
}
const string_set target::remove_directory( const file_type type, const string &directory )
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
