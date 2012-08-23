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
 * Ambrosia/config_baseuration/config_base.h
 * Abstract class for storing config_baseuration options.
 *
 **/

#ifndef AMBROSIA_CONFIG_BASE_H
#define AMBROSIA_CONFIG_BASE_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class file_cache;

class config_base
{
public:
  config_base();
  config_base(toolchain requested_toolchain);
  virtual ~config_base() = 0; // virtual base

  /*
   * Setters
   **********/
  // source directory and file where the object's config_base is attached to
  void set_source_directory(const std::string& source_directory);
  void set_project_file(const std::string& project_file);
  bool add_config(const std::string& config);
  bool remove_config(const std::string& config );

  // Environment PATH
  const string_vector m_environment_PATH;
  // build platform identification
  architecture m_build_architecture;
  environment m_build_environment;
  os m_build_os;
  toolchain m_build_toolchain; // useful for building tools used in the build

  // target platform identification
  architecture m_target_architecture;
  os m_target_os;
  toolchain m_target_toolchain;

  std::string m_source_directory;

  string_set m_config;
  std::string m_project_file;
  std::string m_build_directory; // if source and build dir are equal, this is ./build

private:
  // Platform detection functions
  architecture detect_build_architecture() const;
  environment detect_build_environment() const;
  toolchain detect_toolchain(toolchain requested_toolchain = toolchain::GNU) const;
  void initialize_config();
};

libambrosia_namespace_end

#endif // CONFIG_BASE_H
