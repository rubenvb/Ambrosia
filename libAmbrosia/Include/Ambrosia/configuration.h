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

#ifndef AMBROSIA_CONFIGURATION_H
#define AMBROSIA_CONFIGURATION_H

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

  // source directory and file where the object's configuration is attached to
  void set_source_directory(const std::string& source_directory);

  // Environment PATH
  const string_vector environment_PATH;
  // build platform identification
  architecture build_architecture;
  os build_os;
  toolchain build_toolchain; // useful for building tools used in the build

  // target platform identification
  architecture target_architecture;
  os target_os;
  toolchain target_toolchain;

  std::string source_directory;

  std::string name; // specified by NAME in target
  std::set<file_type> source_types; // source files present to decide which build commands to run
  std::string project_file;
  std::string build_directory; // if source and build dir are equal, this is ./build

  string_map user_variables; // user string --> value
  string_set config_strings;

  std::string gnu_prefix;

private:
  // Platform detection functions
  toolchain detect_toolchain() const;
  void initialize_config();
};

libambrosia_namespace_end

#endif // AMBROSIA_CONFIGURATION_H
