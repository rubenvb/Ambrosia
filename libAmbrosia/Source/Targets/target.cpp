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
 * Ambrosia/Targets/target.cpp
 * Class implementation
 *
 **/

// Class include
#include "Ambrosia/Targets/target.h"

// Ambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/nectar_error.h"
#include "Ambrosia/file_cache.h"

// C++ includes
#include <cstddef>
  using std::size_t;
#include <string>
  using std::string;

libambrosia_namespace_begin

target::target(const string& name,
               const ::libambrosia::configuration& configuration,
               const target_type type,
               const dependency_set& dependencies)
: name(name),
  configuration(configuration),
  type(type),
  dependencies(dependencies),
  files(),
  source_directories(),
  parallel_commands(),
  link_command()
{   }

void target::add_source_file(const file_type general_type,
                             const string& filename,
                             file_cache& file_cache,
                             const string& /*nectar_file*/,
                             const size_t /*line_number*/)
{
  // add source file type to list
  // search specific file_type directories
  const file_type specific_type = detect_type(general_type, filename);
  string_set& directories = source_directories[specific_type];
  string_set& general_directories = source_directories[general_type];
  directories.insert(std::begin(general_directories), std::end(general_directories));
  debug(debug::files) << "target::add_source_file::Finding " << file_type_map_inverse.at(specific_type) << " files matching " << filename << " in:\n"
                      << directories << "\n";
  file_cache.find_source_files(filename, configuration.source_directory, directories, files[specific_type]);
  configuration.source_types.insert(specific_type);
  //if(general_type != specific_type)
  //  file_cache.find_source_files(filename, configuration.source_directory, general_directories, files[general_type]);
}
bool target::add_source_directory(const file_type type,
                                  const string& directory,
                                  file_cache& file_cache)
{
  debug(debug::target) << "target::add_source_directory::Adding directory " << directory << " of type " << file_type_map_inverse.at(type) << ".\n";
  if(!file_cache.add_source_directory(configuration.source_directory / directory))
    return false;
  if(!source_directories[type].insert(directory).second)
    debug(debug::target) << "target::add_source_directory::Directory " << directory << " already present.\n";

  const file_type general_type = get_general_type(type);
  if(type != general_type)
  {
    debug(debug::target) << "target::add_source_directory::Adding directory " << directory << " of general type " << file_type_map_inverse.at(type) << ".\n";
    if(!source_directories[general_type].insert(directory).second)
      debug(debug::target) << "target::add_source_directory::Directory " << directory << " already present.\n";
  }

  return true;
}

void target::add_library(const string& library,
                         const string& /*nectar_file*/,
                         const size_t /*line_number*/)
{
  libraries.insert(library);
}

libambrosia_namespace_end
