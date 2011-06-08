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
    m_source_files()
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
const target_type & target::type() const
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
bool target::add_config( const string &config )
{
    return m_build_config.add_config( config );
}
bool target::remove_config( const string &config )
{
    return m_build_config.remove_config( config );
}

bool target::add_file( const file_type type, const string &filename )
{
    file_set matches;


    return false;
}
bool target::remove_file( const file_type type, const string &filename )
{
    return false;
}
bool target::add_directory( const file_type type, const string &directory )
{
    return false;
}
bool target::remove_directory( const file_type type, const string &directory )
{
    return false;
}

void target::set_output_name( const std::string &name )
{

}


libambrosia_namespace_end
