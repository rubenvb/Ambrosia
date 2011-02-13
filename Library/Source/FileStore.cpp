/**
  * FileStore.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "FileStore.h"

// libAmbrosia includes
#include "Debug.h"
#include "Platform.h"
/* "Typedefs.h" */
    using ambrosia::fileSet;

// C++ includes
#include <iterator>
    using std::insert_iterator;
/* <string> */
    using std::string;

namespace ambrosia
{

    FileStore::FileStore( const string &sourceDirectory )
    :   m_sourceDirectory( sourceDirectory ),
        m_fileList( generateFileList() )
    {   }

    const fileSet FileStore::generateFileList() const
    {
        fileSet files;

        Debug() << "Generating file list in " << m_sourceDirectory << "\n";

        ambrosia::scanDir( insert_iterator<fileSet>(files, files.begin()),
                           m_sourceDirectory );

        Debug() << "Found " << files.size() << " files in "
                  << m_sourceDirectory << "\n";

        return files;
    }
} // namespace ambrosia
