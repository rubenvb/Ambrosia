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
 * Ambrosia/Targets/target.h
 * Abstract target base class.
 *
 **/

#ifndef AMBROSIA_TARGETS_TARGET_H
#define AMBROSIA_TARGETS_TARGET_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iterator>
#include <string>

libambrosia_namespace_begin

class target
{
public:
  target(const std::string& name,
         const configuration& configuration,
         const target_type type,
         const dependency_set& dependencies = dependency_set());

  // Target information
  bool add_source_directory(const file_type type,
                            const std::string& directory,
                            file_cache& file_cache);
  void add_source_file(const file_type general_type,
                       const std::string& filename,
                       file_cache& file_cache,
                       const std::string& nectar_file,
                       const std::size_t line_number);
  bool add_library(const std::string& library,
                   const std::string& nectar_file,
                   const std::size_t line_number);

  // Command generation
  virtual void generate_commands()
  {   }

  std::string name;
  lib::configuration configuration;
  target_type type;
  const dependency_set dependencies;

  std::map<file_type, build_element_set> files;
  std::map<file_type, string_set> source_directories;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_TARGET_H
