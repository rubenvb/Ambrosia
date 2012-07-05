/**
  * Ambrosia/file_cache.h
  * Cache for filesystem. Stores source and build directories
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_FILE_CACHE_H
#define AMBROSIA_FILE_CACHE_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class config_base;

class file_cache
{
public:
  file_cache();

  // Finders
  const std::string find_nectar_file(const std::string& directory,
                                     config_base* config);
  bool find_project_file(const std::string& path,
                         config_base* config);

  // Getters
  const file_set& get_source_file_set(const std::string& directory);

  // Find the unique relative path to the filename, otherwise return all matching duplicates
  const file_set find_source_file(const std::string& filename,
                                  const config_base* config,
                                  const string_set& directories = {});
  // Match filename with all directories and match wildcards
  const file_set match_source_files(const std::string& filename,
                                    const config_base* config,
                                    const string_set& directories = {});
  // Read directory contents from disk (don't complain if already present).
  void add_source_directory(const std::string& directory = "");
  void add_build_directory(const std::string& directory);

private:
  map_string_file_set m_source_files; // all files in s_ambrosia_config::m_source_directory
  map_string_file_set m_build_files;  // all files in s_ambrosia_config::m_build_directory
};

extern file_cache s_file_cache;

libambrosia_namespace_end

#endif // AMBROSIA_FILE_CACHE_H
