/**
 *
 * Project Ambrosia: Ambrosia commandline build tool
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
 * commandline.h
 * Process commandline options and modify configuration accordingly.
 *
 **/

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

// Global include
#include "global.h"

// Ambrosia includes
#include "program_options.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// Foward declarations
libambrosia_namespace_begin
class configuration;
class program_options;
class project;
libambrosia_namespace_end

ambrosia_namespace_begin

void apply_commandline_options(const string_vector& options,
                               program_options& program_options,
                               lib::project& project);
// trim argument="arg<char>value" to "arg" and return "value". If argument="arg", return empty string.
const std::string split_argument(std::string& argument,
                                 const char split_char,
                                 const size_t argument_number);

void add_build_target(program_options& options,
                      const std::string& target,
                      const string_set& config_strings = string_set());

void set_program_option(program_options& options,
                        const std::string& option,
                        const std::size_t argument_number);

void set_ambrosia_option(lib::project& project,
                         const std::string& option,
                         const std::string& value,
                         const std::size_t argument_number);

void set_ambrosia_cross(lib::configuration& configuration,
                        const std::string& cross,
                        const std::size_t argument_number);

bool add_configuration_options(const std::string& options,
                               lib::configuration& configuration);

void add_external_dependency(const std::string& name,
                             const std::string& location,
                             lib::project& project);

ambrosia_namespace_end

#endif // COMMANDLINE_H
