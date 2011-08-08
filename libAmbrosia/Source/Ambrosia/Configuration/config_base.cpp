/**
  * Ambrosia/config_baseuration/config_base.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Configuration/config_base.h"

// libambrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/file_store.h"
#include "Ambrosia/platform.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

config_base::config_base()
:   m_target_os( build_os ),
    m_target_architecture( build_architecture ),
    m_target_toolchain( toolchain::GNU ),
    m_config(),
    m_source_directory(),
    m_project_file(),
    m_build_directory()
{   }
config_base::~config_base()
{   }

/*
 * Setters
 **********/
bool config_base::set_source_directory( const string &source_directory )
{
    if( !directory_exists(source_directory) )
        return false;

    debug(5) << "config_base::set_source_directory::Setting source directory to: " << source_directory << "\n";
    m_source_directory = source_directory;
    debug(5) << "config_base::set_source_directory::Adding " << m_source_directory << " to s_file_store.\n";
    s_file_store.add_source_directory( m_source_directory );
    return true;
}
void config_base::set_project_file( const string &project_file )
{
    m_project_file = project_file;
}
bool config_base::add_config( const string &config )
{
    return m_config.insert( config ).second;
}
bool config_base::remove_config( const string &config )
{
    return m_config.erase( config );
}
/*
 * Getters
 **********/
string_set & config_base::config()
{
    return m_config;
}
const string_set & config_base::config() const
{
    return m_config;
}
const string & config_base::source_directory() const
{
    return m_source_directory;
}
const string & config_base::project_file() const
{
    return m_project_file;
}
const std::string config_base::path_to_project_file() const
{
    return m_project_file;
}
const string & config_base::build_directory() const
{
    return m_build_directory;
}
const os & config_base::target_os() const
{
    return m_target_os;
}
const architecture & config_base::target_architecture() const
{
    return m_target_architecture;
}
const toolchain & config_base::target_toolchain() const
{
    return m_target_toolchain;
}

libambrosia_namespace_end
