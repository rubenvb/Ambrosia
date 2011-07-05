/**
  * Ambrosia/build_config.h
  * Information required to set up a chain of build commands from the Target list.
  *  It contains the following:
  *   - cross-compile prefix,
  *   - targets to be built (will be resolved for dependencies later)
  *   - target-specific configurations (release/debug/...)
  *   - user options (double dash '--' options
  *   -...
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef CONFIGURATION_BUILD_CONFIG_H
#define CONFIGURATION_BUILD_CONFIG_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Configuration/config.h"
#include "enums.h"
#include "typedefs.h"

// C++ includes
/* <map> */
/* <string> */
/* <utility> */

libambrosia_namespace_begin

class build_config : config
{
public:
    build_config();
    build_config( ambrosia_config & );

    // Setters
    bool add_config( const std::string &config );
    bool remove_config( const std::string &config );

    // Getters
    const string_set & config() const;
    string_set & config();
    bool contains( const std::string &config ) const;
    const string_map & user_options() const;

private:
    string_set m_config; // CONFIG
    string_map m_user_variables; // user string --> value
};

libambrosia_namespace_end

#endif // CONFIGURATION_BUILDCONFIG_H
