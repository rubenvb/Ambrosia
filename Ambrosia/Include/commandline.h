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
#include "Ambrosia/typedefs.h"

// Foward declarations
libambrosia_namespace_begin
class ambrosia_config;
class file_cache;
class project;
libambrosia_namespace_end

ambrosia_namespace_begin

void apply_commandline_options( const string_vector &options, lib::project &project,
                                lib::file_cache &files);

bool add_build_target( const std::string &target );

void set_program_option( const std::string &option, const std::string &value );

bool add_configuration_options( const std::string &options, lib::ambrosia_config* config );

ambrosia_namespace_end

#endif // COMMANDLINE_H
