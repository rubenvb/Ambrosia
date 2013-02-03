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
 * Ambrosia/file_cache.h
 * Cache for filesystem. Stores source and build directories
 *
 **/

#ifndef AMBROSIA_FILE_CACHE_H
#define AMBROSIA_FILE_CACHE_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <functional>

namespace ambrosia
{
namespace lib
{

// Forward declarations
class configuration;

class file_cache
{
public:
  file_cache();

  // Getters
  const file_set& get_source_file_set(const std::string& directory);

  // Find the unique relative path to the filename, otherwise return all matching duplicates
  void find_source_files(const std::string& filename,
                         const std::string& source_directory,
                         const string_set& subdirectories,
                         build_element_set& files);
  // Match filename with all directories and match wildcards
  const file_set match_source_files(const std::string& filename,
                                    const configuration* config,
                                    const string_set& directories = string_set());
  // Find
  // Read directory contents from disk (don't complain if already present).
  bool add_source_directory(const std::string& directory);
  void add_build_directory(const std::string& directory);

private:
  map_string_file_set m_source_files; // all files in configuration::m_source_directory
  map_string_file_set m_build_files;  // all files in configuration::m_build_directory
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_FILE_CACHE_H
