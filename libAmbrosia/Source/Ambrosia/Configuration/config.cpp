/**
  * Ambrosia/Configuration/config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Configuration/config.h"

// libambrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/file_store.h"
#include "Ambrosia/platform.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

config::config()
:   m_target_os( build_os ),
    m_target_architecture( build_architecture ),
    m_target_toolchain( toolchain::GNU ),
    m_config(),
    m_source_directory(),
    m_project_file(),
    m_build_directory()
{   }
config::~config()
{   }

/*
 * Setters
 **********/
bool config::set_source_directory( const string &source_directory )
{
    if( !directory_exists(source_directory) )
        return false;

    debug(5) << "config::set_source_directory::Setting source directory to: " << source_directory << "\n";
    m_source_directory = source_directory;
    debug(5) << "config::set_source_directory::Adding " << m_source_directory << " to s_file_store.\n";
    s_file_store.add_source_directory( "" ); // directory equals m_source_directory + argument, which needs to be empty here
    return true;
}
void config::set_project_file( const string &project_file )
{
    m_project_file = project_file;
}
bool config::add_config( const string &config )
{
    return m_config.insert(config).second;
}
bool config::remove_config( const string &config )
{
    return m_config.erase(config);
}
/*
 * Getters
 **********/
const string & config::source_directory() const
{
    return m_source_directory;
}
const string & config::project_file() const
{
    return m_project_file;
}
const std::string config::path_to_project_file() const
{
    return m_source_directory + "/" + m_project_file;
}
const string & config::build_directory() const
{
    return m_build_directory;
}
const os & config::target_os() const
{
    return m_target_os;
}
const architecture & config::target_architecture() const
{
    return m_target_architecture;
}
const toolchain & config::target_toolchain() const
{
    return m_target_toolchain;
}


libambrosia_namespace_end
