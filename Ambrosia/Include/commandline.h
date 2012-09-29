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

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// Foward declarations
libambrosia_namespace_begin
class configuration;
class file_cache;
class project;
libambrosia_namespace_end

ambrosia_namespace_begin

enum class internal_option
{
  dump_commands
  //...
};
extern std::multimap<internal_option, std::string> internal_option_map;
extern std::multimap<std::string, internal_option> internal_option_map_reverse;

enum class internal_value_option
{
  debug,
  gnu_prefix
};

void apply_commandline_options(const string_vector& options,
                               lib::project& project);

void add_build_target(lib::project& project,
                      const std::string& target,
                      const string_set& options = string_set());

void set_internal_option(const std::string& option,
                         const size_t argument_number);
void set_internal_value_option(const std::string& option,
                               const std::string& value,
                               const size_t argument_number);

void set_program_option(const std::string& option,
                        const std::string& value);

bool add_configuration_options(const std::string& options,
                               lib::configuration& config);

ambrosia_namespace_end

#endif // COMMANDLINE_H
