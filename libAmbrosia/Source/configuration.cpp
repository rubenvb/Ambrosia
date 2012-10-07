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
 * Ambrosia/configurationuration/configuration.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/configuration.h"

// libambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/boost_wrapper.h"
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

configuration::configuration()
: environment_PATH(platform::get_environment_PATH()),
  build_architecture(platform::build_architecture),
  build_environment(detect_build_environment()),
  build_os(platform::build_os),
  build_toolchain(detect_toolchain()),
  target_architecture(build_architecture),
  target_os(build_os),
  target_toolchain(detect_toolchain()),
  source_directory(),
  config_strings(),
  project_file(),
  build_directory("./build")
{
  initialize_config();
  debug(debug::config) << "\nconfiguration::config contains:\n" << config_strings << "\n";
}
configuration::configuration(toolchain requested_toolchain)
: environment_PATH(platform::get_environment_PATH()),
  build_architecture(platform::build_architecture),
  build_environment(detect_build_environment()),
  build_os(platform::build_os),
  build_toolchain(detect_toolchain()),
  target_architecture(build_architecture),
  target_os(build_os),
  target_toolchain(detect_toolchain(requested_toolchain)),
  source_directory(),
  config_strings(),
  project_file(),
  build_directory()
{
  initialize_config();
}

void configuration::set_source_directory(const string& source_directory)
{
  if(!platform::directory_exists(source_directory))
    throw internal_error("Attempting to call configuration::set_source_directory with a non-existent directory: " + source_directory);

  debug(debug::config) << "configuration::set_source_directory::Setting source directory to: " << source_directory << "\n";
  this->source_directory = source_directory;
  debug(debug::config) << "configuration::set_source_directory::Adding " << source_directory << " to s_file_store.\n";
}
/*void configuration::set_project_file(const string& project_file)
{
  this->project_file = project_file;
}*/
/*bool configuration::add_config(const string& config_string)
{
  return config_strings.insert(config_string).second;
}
bool configuration::remove_config(const string& config_string)
{
  // MSVC C4800 without the "0 !="
  return 0 != config_strings.erase(config_string);
}*/

// Platform detection functions
environment configuration::detect_build_environment() const
{
  emit_warning("configuration::detect_build_environment::not quite implemented completely yet.");
#if _WIN32
  return environment::cmd;
#else
  return environment::bash;
#endif
}
toolchain configuration::detect_toolchain(toolchain requested_toolchain) const
{
  //TODO check toolchain availability, but not here, as gnu_prefix etc could alter the name of the executables.
  emit_warning("detect_toolchain needs to validate input.");
  return requested_toolchain;
}
void configuration::initialize_config()
{
  config_strings =
    list_entries_begin
      entry_begin os_map_inverse.at(target_os) entry_end // Target OS
      entry_begin architecture_map_inverse.at(target_architecture) entry_end // Target architecture
      entry_begin toolchain_map_inverse.at(target_toolchain) entry_end // Toolchain
      entry_begin "build_" + os_map_inverse.at(build_os) entry_end // Build OS
      entry_begin "build_" + architecture_map_inverse.at(build_architecture) entry_end // Build architecture
      entry_begin environment_map_inverse.at(build_environment) entry_end // Shell environment
      entry_begin "debug" entry_end // debug build is the default
    entries_end;
  // Convenience config strings
  if(target_os == os::Windows && target_toolchain == toolchain::GNU)
    config_strings.insert("mingw");
  if(target_os == os::Linux || target_os == os::MacOSX)
    config_strings.insert("unix");
}

libambrosia_namespace_end
