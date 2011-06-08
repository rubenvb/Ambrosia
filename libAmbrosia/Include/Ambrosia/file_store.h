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

private:
    file_set m_source_files;
    file_set m_build_files;
};

libambrosia_namespace_end

#endif // FILE_STORE_H
