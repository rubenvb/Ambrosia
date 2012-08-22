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
 * Ambrosia/Configuration/ambrosia_config.h
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Configuration/ambrosia_config.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/commandline_error.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
#include <stdexcept>
  using std::logic_error;
#include <string>
  using std::string;

libambrosia_namespace_begin

/*
 * Setters
 **********/
void ambrosia_config::set_ambrosia_cross(const std::string& cross,
                                         const size_t argument_number)
{
  debug(debug::config) << "ambrosia_config::Checking and setting cross-compilation options through Ambrosia specification.\n";

  // verify format
  if(!wildcard_compare("*-*-*", cross))
    throw commandline_error("Ambrosia cross-compile specification should be of the form \'OS-Architecture-Toolchain\'.\n", argument_number);
  else
    debug(debug::config) << "ambrosia_config::cross has correct format.\n";

  // find relevant parts and complain if somethin's wrong
  const string::size_type architecture_index = cross.find("-") + 1;
  const string::size_type toolchain_index = cross.find("-", architecture_index) + 1;

  // split up the string
  const string os_string(cross.substr(0, architecture_index-1));
  const string architecture_string(cross.substr(architecture_index, toolchain_index-architecture_index-1));
  const string toolchain_string(cross.substr(toolchain_index, string::npos));
  debug(debug::config) << "ambrosia_config::cross options specified:\n"
                       << "              os = " << os_string << ".\n"
                       << "              architecture = " << architecture_string << ".\n"
                       << "              toolchain = " << toolchain_string << ".\n";

  // set the appropriate internal options
  os new_os = build_os; // shut up uninitialized warning
  if(!map_value(os_map, os_string, new_os))
    throw commandline_error("Specified invalid target OS: " + os_string, argument_number);
  else
    m_target_os = new_os;
  architecture new_architecture = build_architecture; // shut up uninitialized warning
  if(map_value(architecture_map, architecture_string, new_architecture))
    throw commandline_error("Specified invalid target bitness: " + architecture_string, argument_number);
  else
    m_target_architecture = new_architecture;
  toolchain new_toolchain = toolchain::GNU; // shut up uninitialized warning
  if(map_value(toolchain_map, toolchain_string, new_toolchain))
    throw commandline_error("Specified invalid target toolchain: " + toolchain_string, argument_number);
  else
    m_target_toolchain = new_toolchain;
}
void ambrosia_config::add_target_config_options(const std::string& target,
                                                const string_set& options)
{
  string_set duplicates = merge_sets(m_target_config_options[target], options);
  if(!duplicates.empty())
    emit_warning("");

  throw error("add_target_config_options is unimplemented.");
}

const map_string_set_string& ambrosia_config::target_config_options() const
{
  return m_target_config_options;
}

libambrosia_namespace_end
