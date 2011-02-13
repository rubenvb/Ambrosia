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
        FileStore( const std::string &sourceDir );

    private:
        // Project files
        std::string m_sourceDirectory; // relative or absolute source directory
        ambrosia::fileSet m_fileList; // list of files in source directory
        const ambrosia::fileSet generateFileList() const;
    };
} // namespace ambrosia

#endif // FILESTORE_H
