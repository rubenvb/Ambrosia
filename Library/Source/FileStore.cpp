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
    using ambrosia::file_set;

// C++ includes
#include <iterator>
    using std::insert_iterator;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    FileStore::FileStore( const string &source_directory )
    :   m_source_directory( source_directory ),
        m_file_list( generate_file_list() )
    {   }

    const string FileStore::find_nectar_file( const std::string &directory )
    {
        Debug() << "FileStore::find_nectar_file called.\n";
        vector<string> file_list;
        scan_directory( std::back_inserter(file_list), directory );
        Debug() << "FileStore::found " << file_list.size() << " files:\n";
        const auto end = file_list.end();
        string file;
        for( auto it = file_list.begin(); it != end; ++it )
        {
            file = (*it);
            Debug() << "FileStore::searching for nectar.txt file... " << file << "\n";
            // find the first *.nectar.txt file in main source directory (not a subdirectory)
            size_t index = file.rfind( ".nectar.txt" );
            if( (index < string::npos) && (file.find('/') == string::npos) )
                break;
        }
        return file;

    }

    const file_set FileStore::generate_file_list() const
    {
        file_set files;

        Debug() << "Generating file list in " << m_source_directory << "\n";

        ambrosia::recursive_scan_directory( insert_iterator<file_set>(files, files.begin()),
                           m_source_directory );

        Debug() << "Found " << files.size() << " files in "
                  << m_source_directory << "\n";

        return files;
    }
} // namespace ambrosia
