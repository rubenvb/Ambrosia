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
 * Ambrosia/target.h
 * Ambrosia's internal representation of a build target. This includes:
 *  - full nectar.txt target text for:
 *     > multiple builds in one
 *     > decent error reporting referring to line number and text
 *  - full source file list (including headers etc.)
 *  - inter-target dependencies
 *  - ...
 *
 **/

#ifndef AMBROSIA_TARGET_H
#define AMBROSIA_TARGET_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/Configuration/build_config.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/node.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>
#include <utility>
#include <vector>

libambrosia_namespace_begin

class target : public node
{
public:
  // 'global' target with subproject-wide dependencies
  target(const std::string& subdirectory,
         const dependency_set& dependencies,
         const ambrosia_config& config,
         file_cache& cache);
  // other targets are based off of global's build_config
  target(const std::string& subdirectory,
         const std::string& name,
         const target_type type,
         const dependency_set& dependencies,
         const build_config& config,
         file_cache& cache);

  // file modifiers and accessor
  void add_source_file(const file_type type,
                       const std::string& filename,
                       const std::string& nectar_file,
                       const size_t line_number);
  void remove_source_file(const file_type type,
                   const std::string& filename );
  bool add_source_directory(const file_type type,
                            const std::string& directory);
  void remove_source_directory(const file_type type,
                        const std::string& directory);
  const build_element_set& files(const file_type type) const;

  // libraries to be linked
  bool add_library(const std::string& library);
  void remove_library(const std::string& library);

  // output object filenames
  void generate_object_filenames();

  // variables
  build_config m_build_config; // build configuration, inherited from global target's build_config
  const dependency_set m_dependencies; // dependency+type
  std::string m_output_name;
  const target_type m_type; // target type

private:
  file_cache& m_file_cache; // project's file_cache
  map_file_type_string_set m_source_directories; // source directories per file type
  map_file_type_build_element_set m_files; // source and object files per file type with last modified time
  std::string m_output_file;
  string_set m_libraries; // libraries to be linked
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGET_H
