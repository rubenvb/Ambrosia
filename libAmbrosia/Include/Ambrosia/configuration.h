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
 * Ambrosia/configurationuration/configuration.h
 * Abstract class for storing configurationuration options.
 *
 **/

#ifndef configurationURATION_H
#define configurationURATION_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class file_cache;

class configuration
{
public:
  configuration();
  configuration(toolchain requested_toolchain);

  // parse Ambrosia cross target string and set options
  void set_ambrosia_cross(const std::string& cross,
                          const size_t argument_number);

  // source directory and file where the object's configuration is attached to
  void set_source_directory(const std::string& source_directory);
  // add target or add additional config options to one target
  void add_target_config_options(const std::string& target,
                                 const string_set& options);

  // Environment PATH
  const string_vector environment_PATH;
  // build platform identification
  architecture build_architecture;
  environment build_environment;
  os build_os;
  toolchain build_toolchain; // useful for building tools used in the build

  // target platform identification
  architecture target_architecture;
  os target_os;
  toolchain target_toolchain;

  std::string source_directory;

  std::string name; // specified by NAME in target
  std::set<file_type> source_types; // source files present to decide which build commands to run
  string_map user_variables; // user string --> value
  string_set config_strings;
  std::string project_file;
  std::string build_directory; // if source and build dir are equal, this is ./build

  bool dump_commands; // TODO: find better place for this
  std::string gnu_prefix;

private:
  // Platform detection functions
  architecture detect_build_architecture() const;
  environment detect_build_environment() const;
  toolchain detect_toolchain(toolchain requested_toolchain = ambrosia_toolchain) const;
  void initialize_config();
};

libambrosia_namespace_end

#endif // configurationURATION_H