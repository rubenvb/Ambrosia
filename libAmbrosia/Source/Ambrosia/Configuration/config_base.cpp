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
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/file_store.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

config_base::config_base()
:   m_environment_PATH( get_environment_PATH() ),
    m_build_architecture( build_architecture ),
    m_build_environment( detect_build_environment() ),
    m_build_os( build_os ), // global from Ambrosia/platform.h
    m_build_toolchain( detect_toolchain() ),
    m_target_architecture( m_build_architecture ),
    m_target_os( m_build_os ),
    m_target_toolchain( detect_toolchain() ),
    m_source_directory(),
    m_config(),
    m_project_file(),
    m_build_directory()
{
    initialize_config();
    debug(4) << "\nconfig_base::config contains:\n" << m_config << "\n";
}
config_base::config_base( toolchain requested_toolchain )
:   m_environment_PATH( get_environment_PATH() ),
    m_build_architecture( build_architecture ),
    m_build_environment( detect_build_environment() ),
    m_build_os( build_os ), // global from Ambrosia/platform.h
    m_build_toolchain( detect_toolchain() ),
    m_target_architecture( m_build_architecture ),
    m_target_os( m_build_os ),
    m_target_toolchain( detect_toolchain(requested_toolchain) ),
    m_source_directory(),
    m_config(),
    m_project_file(),
    m_build_directory()
{
    initialize_config();
}
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
    if( !error_status() )
        return true;
    else
        return false;
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

// Platform detection functions
environment config_base::detect_build_environment() const
{
    return environment::cmd;
}
toolchain config_base::detect_toolchain() const
{
    return toolchain::GNU;
}
toolchain config_base::detect_toolchain( toolchain requested_toolchain ) const
{
    emit_error( "detect_toolchain needs to validate input" );
    return requested_toolchain;
}
void config_base::initialize_config()
{
    m_config = {
    // Target OS
        map_value(os_map_inverse, m_target_os),
    // Target Architecture
        map_value(architecture_map_inverse, m_target_architecture),
    // Toolchain
        map_value(toolchain_map_inverse, m_target_toolchain),
    // Build OS
        "build_" + map_value(os_map_inverse, m_build_os),
    // Build architecture
        "build_" + map_value(architecture_map_inverse, m_build_architecture),
    // Shell environment
        map_value(environment_map_inverse, m_build_environment)
        };
    // Convenience config strings
    if( m_target_os == os::Windows && m_target_toolchain == toolchain::GNU )
        m_config.insert( "mingw" );
    if( m_target_os == os::Linux || m_target_os == os::MacOSX )
        m_config.insert( "unix" );
}

libambrosia_namespace_end
