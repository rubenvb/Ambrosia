/**
 *
 * Project Ambrosia: Ambrosia library
 *
 * Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related
 * and neighboring rights to this software to the public domain worldwide. This software is
 * distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Ambrosia/config_baseuration/config_base.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Configuration/config_base.h"

// libambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

config_base::config_base()
: m_environment_PATH(get_environment_PATH()),
  m_build_architecture(build_architecture),
  m_build_environment(detect_build_environment()),
  m_build_os(build_os), // global from Ambrosia/platform.h
  m_build_toolchain(detect_toolchain()),
  m_target_architecture(m_build_architecture),
  m_target_os(m_build_os),
  m_target_toolchain(detect_toolchain()),
  m_source_directory(),
  m_config(),
  m_project_file(),
  m_build_directory()
{
  initialize_config();
  debug(debug::config) << "\nconfig_base::config contains:\n" << m_config << "\n";
}
config_base::config_base(toolchain requested_toolchain)
: m_environment_PATH(get_environment_PATH()),
  m_build_architecture(build_architecture),
  m_build_environment(detect_build_environment()),
  m_build_os(build_os), // global from Ambrosia/platform.h
  m_build_toolchain(detect_toolchain()),
  m_target_architecture(m_build_architecture),
  m_target_os(m_build_os),
  m_target_toolchain(detect_toolchain(requested_toolchain)),
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
void config_base::set_source_directory(const string& source_directory)
{
  if(!directory_exists(source_directory))
    throw internal_error("Attempting to call config_base::set_source_directory with a non-existent directory: " + source_directory);

  debug(debug::config) << "config_base::set_source_directory::Setting source directory to: " << source_directory << "\n";
  m_source_directory = source_directory;
  debug(debug::config) << "config_base::set_source_directory::Adding " << m_source_directory << " to s_file_store.\n";
  s_file_cache.add_source_directory(m_source_directory);
}
void config_base::set_project_file(const string& project_file)
{
  m_project_file = project_file;
}
bool config_base::add_config(const string& config)
{
  return m_config.insert(config).second;
}
bool config_base::remove_config(const string& config)
{
  return m_config.erase(config);
}

// Platform detection functions
environment config_base::detect_build_environment() const
{
  emit_warning("config_base::detect_build_environment::not quite implemented completely yet.");
#if _WIN32
  return environment::cmd;
#else
  return environment::bash;
#endif
}
toolchain config_base::detect_toolchain(toolchain requested_toolchain) const
{
  //TODO check toolchain availability, but not here, as gnu_prefix etc could alter the name of the executables.
  emit_warning("detect_toolchain needs to validate input.");
  return requested_toolchain;
}
void config_base::initialize_config()
{
  m_config = { map_value(os_map_inverse, m_target_os), // Target OS
               map_value(architecture_map_inverse, m_target_architecture), // Target Architecture
               map_value(toolchain_map_inverse, m_target_toolchain), // Toolchain
               "build_" + map_value(os_map_inverse, m_build_os), // Build OS
               "build_" + map_value(architecture_map_inverse, m_build_architecture), // Build architecture
               map_value(environment_map_inverse, m_build_environment) // Shell environment
             };
  // Convenience config strings
  if( m_target_os == os::Windows && m_target_toolchain == toolchain::GNU )
    m_config.insert( "mingw" );
  if( m_target_os == os::Linux || m_target_os == os::MacOSX )
    m_config.insert( "unix" );
}

libambrosia_namespace_end
