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
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

class file_store
{
public:
    file_store();

    // getters
    const file_set & get_source_file_set( const std::string &directory );

    // Find the unique relative path to the filename, otherwise return all matching duplicates
    const file_set find_source_file( const std::string &filename, const string_set &directories = string_set() );
    // Match filename with all directories and match wildcards
    const file_set match_source_files( const std::string &filename, const string_set &directories = string_set() );
    // Read directory contents from disk (don't complain if already present).
    void add_source_directory( const std::string &directory = "" );
    void add_build_directory(const std::string &directory );

private:
    map_string_file_set m_source_files; // all files in s_ambrosia_config::m_source_directory
    map_string_file_set m_build_files;  // all files in s_ambrosia_config::m_build_directory
};

extern file_store s_file_store;

libambrosia_namespace_end

#endif // FILE_STORE_H
