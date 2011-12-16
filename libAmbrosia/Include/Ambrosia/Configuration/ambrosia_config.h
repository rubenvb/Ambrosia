/**
  * Ambrosia/Configuration/ambrosia_config.h
  * Ambrosia's runtime configuration, used as a basis for command generation and project configuration.
  *  This includes:
  *   - build and target platform and toolchain detection.
  *   - initial CONFIG listing.
  *   - ... (anything
  *
  * Author: Ruben Van Boxem
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
/* <string> */

libambrosia_namespace_begin

class ambrosia_config : public config_base
{
public:
    ambrosia_config();

    // Setters
    void set_ambrosia_cross( const std::string &cross );
    void set_gnu_prefix( const std::string &prefix ); // handles a lot of standard GU triplets
    // add target or add additional config options to one target
    void add_target_config_options( const std::string &target, const string_set &options );
    // add config options to all present targets
    void add_general_config( const string_set &options );
    // add user option settings
    void set_user_option( const std::string &option, const std::string &value );

    // getters;
    const map_string_set_string & target_config_options() const;

private:
    map_string_set_string m_target_config_options;
};

//extern ambrosia_config s_ambrosia_config;

libambrosia_namespace_end

#endif // AMBROSIA_CONFIGURATION_AMBROSIA_CONFIG_H
