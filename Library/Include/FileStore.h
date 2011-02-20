/**
  * FileStore.h
  * Internal list of files and directories that might be relevant to the
  *  project being built.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef FILESTORE_H
#define FILESTORE_H

// Global include
#include "Ambrosia/Global.h"

// libAmbrosia includes
#include "Ambrosia/Typedefs.h"

// C++ includes
/* <string> */

namespace ambrosia
{
    class FileStore
    {
    public:
        FileStore( const std::string &source_directory );

        static const std::string find_nectar_file( const std::string &directory );

    private:
        // Project files
        std::string m_source_directory; // relative or absolute source directory
        file_set m_file_list; // list of files in source directory
        const file_set generate_file_list() const;
    };
} // namespace ambrosia

#endif // FILESTORE_H
