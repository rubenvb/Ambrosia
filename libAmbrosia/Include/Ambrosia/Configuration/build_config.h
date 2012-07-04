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

#ifndef AMBROSIA_CONFIGURATION_BUILD_CONFIG_H
#define AMBROSIA_CONFIGURATION_BUILD_CONFIG_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/config_base.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
/* <map> */
/* <string> */
/* <utility> */

libambrosia_namespace_begin

// Forward declarations
class ambrosia_config;

class build_config : public config_base
{
public:
    build_config( const std::string &subdirectory, const config_base &config );

    const string_map & user_options() const;

    friend class target;

private:
    string_map m_user_variables; // user string --> value
};

libambrosia_namespace_end

#endif // CONFIGURATION_BUILDCONFIG_H
