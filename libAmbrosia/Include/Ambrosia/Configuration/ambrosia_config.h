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
 * Ambrosia's runtime configuration, used as a basis for command generation and project configuration.
 *  This includes:
 *   - build and target platform and toolchain detection.
 *   - initial CONFIG listing.
 *   - ... (anything)
 *
 **/

#ifndef AMBROSIA_CONFIGURATION_AMBROSIA_CONFIG_H
#define AMBROSIA_CONFIGURATION_AMBROSIA_CONFIG_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/config_base.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class ambrosia_config : public config_base
{
public:
  // Setters
  void set_ambrosia_cross(const std::string& cross,
                          const size_t argument_number);
  //void set_gnu_prefix(const std::string& prefix); // handles a lot of standard GNU triplets
  // add target or add additional config options to one target
  void add_target_config_options(const std::string& target,
                                 const string_set& options);
  // add config options to all present targets
  void add_general_config(const string_set& options);
  // add user option settings
  void set_user_option(const std::string& option,
                       const std::string& value);

  // getters;
  const map_string_set_string& target_config_options() const;

  std::string m_gnu_prefix;

private:
  map_string_set_string m_target_config_options;
};

libambrosia_namespace_end

#endif // AMBROSIA_CONFIGURATION_AMBROSIA_CONFIG_H
