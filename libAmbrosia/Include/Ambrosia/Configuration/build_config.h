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
 * Ambrosia/build_config.h
 * Information required to set up a chain of build commands from the Target list.
 *  It contains the following:
 *   - cross-compile prefix,
 *   - targets to be built (will be resolved for dependencies later)
 *   - target-specific configurations (release/debug/...)
 *   - user options (double dash '--' options
 *   -...
 *
 **/

#ifndef AMBROSIA_CONFIGURATION_BUILD_CONFIG_H
#define AMBROSIA_CONFIGURATION_BUILD_CONFIG_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/config_base.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <map>
#include <string>
#include <utility>

libambrosia_namespace_begin

// Forward declarations
class ambrosia_config;

class build_config : public config_base
{
public:
  build_config(const std::string& subdirectory, const config_base& config);

  //build_config(const build_config& config) = delete;

  const string_map& user_options() const;

  std::set<file_type> m_source_types; // source files present to decide which build commands to run

private:
  string_map m_user_variables; // user string --> value
};

libambrosia_namespace_end

#endif // CONFIGURATION_BUILDCONFIG_H
