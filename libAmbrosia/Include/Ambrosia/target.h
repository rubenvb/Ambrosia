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
#include "Ambrosia/configuration.h"
#include "Ambrosia/configuration.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/node.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>
#include <utility>
#include <vector>

libambrosia_namespace_begin

/*class target : public node
{
public:
  target(const target_type type,
         const std::string& name,
         const configuration& configuration,
         const std::string& subdirectory = "",
         const dependency_set& dependencies = dependency_set());
  // 'global' target with subproject-wide dependencies
  target(const std::string& subdirectory,
         const dependency_set& dependencies,
         const configuration& configuration,
         file_cache& cache)
  // other targets are based off of global's configuration
  target(const std::string& subdirectory,
         const std::string& name,
         const target_type type,
         const dependency_set& dependencies,
         const configuration& config);

  // file modifiers and accessor
  void add_source_file(const file_type type,
                       const std::string& filename,
                       file_cache& file_cache,
                       const std::string& nectar_file,
                       const std::size_t line_number);
  void remove_source_file(const file_type type,
                   const std::string& filename);
  bool add_source_directory(const file_type type,
                            const std::string& directory);
  void remove_source_directory(const file_type type,
                        const std::string& directory);
  const string_set source_directories(const file_type type) const
  { return m_source_directories.at(type); }
  const build_element_set& files(const file_type type) const;
  //{ return m_files.at(type); }

  // libraries to be linked
  bool add_library(const std::string& library);
  void remove_library(const std::string& library);

  // output object filenames
  void generate_object_filenames();

  // variables
  configuration configuration; // build configuration, inherited from global target's configuration
  const dependency_set dependencies; // dependency+type
  std::string output_name;
  const target_type type; // target type

private:
  map_file_type_string_set m_source_directories; // source directories per file type
  map_file_type_build_element_set m_files; // source and object files per file type with last modified time
  std::string m_output_file;
  string_set m_libraries; // libraries to be linked
};*/

libambrosia_namespace_end

#endif // AMBROSIA_TARGET_H
