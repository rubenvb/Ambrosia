/**
  * Ambrosia/file_store.h
  * Cache for filesystem. Stores source and build directories
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef FILE_STORE_H
#define FILE_STORE_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "typedefs.h"

libambrosia_namespace_begin

class file_store
{
public:
    file_store();

    // Match filename with all directories and match wildcards, and store duplicates
    void match_source_files( const string_set &directories, const std::string &filename,
                             file_set &matches, file_set &duplicates ) const;
    // Read directory contents from disk (don't complain if already present).
    void add_source_directory( const std::string &directory );
    void add_build_directory( const std::string &directory );

private:
    map_string_file_set m_source_files; // all files in s_ambrosia_config::m_source_directory
    map_string_file_set m_build_files;  // all files in s_ambrosia_config::m_build_directory
};

libambrosia_namespace_end

#endif // FILE_STORE_H
