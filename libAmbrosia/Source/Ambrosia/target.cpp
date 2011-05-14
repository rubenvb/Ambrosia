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
#include "debug.h"
#include "enum_maps.h"

// C++ includes
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
    m_build_config( config )
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
void target::add_config( const string_set &config )
{
}

void add_file( const file_type, const std::string &file );
void add_files( const file_type, const std::set<std::string> &files );
void target::set_output_name( const std::string &name )
{

}


libambrosia_namespace_end
