/**
  * commandline.h
  * Process commandline options and modify ambrosia_config accordingly.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/typedefs.h"

ambrosia_namespace_begin

bool apply_commandline_options( const string_vector &options, lib::ambrosia_config &config );

bool add_build_target( const std::string &target, lib::ambrosia_config &config );

void set_internal_option( const std::string &option, const std::string &value,
                          lib::ambrosia_config &config );

bool add_configuration_options( const std::string &options, lib::ambrosia_config &config );

ambrosia_namespace_end

#endif // COMMANDLINE_H
