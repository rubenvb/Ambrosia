/**
  * file_store.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "file_store.h"

// libAmbrosia includes
#include "error.h"
#include "debug.h"
#include "platform.h"
/* "typedefs.h" */
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
    file_store::file_store( const string &source_directory )
    :   m_source_directory( source_directory ),
        m_file_list( generate_file_list() )
    {   }

    const string file_store::find_nectar_file( const std::string &directory )
    {
        debug() << "file_store::find_nectar_file called.\n";
        vector<string> file_list;
        scan_directory( std::back_inserter(file_list), directory );
        debug() << "file_store::found " << file_list.size() << " files:\n";
        const auto end = file_list.end();
        vector<string> nectar_files;
        for( auto it = file_list.begin(); it != end; ++it )
        {
            const string file( *it );
            // find the first *.nectar.txt file in main source directory (not a subdirectory)
            size_t index = file.rfind( ".nectar.txt" );
            if( (index < string::npos) && (file.find('/') == string::npos) )
            {
                debug() << "file_store::searching for nectar.txt file... " << file << "\n";
                nectar_files.push_back( file );
            }
        }

        if( nectar_files.size() == 1 )
            return nectar_files[0]; // return the unique match
        else if( !nectar_files.empty() )
            error::emit_error( "More than one *.nectar.txt files found in specified directory: " + directory );
        // If there are no or more than one match, return an empty string
        return string();
    }

    const file_set file_store::generate_file_list() const
    {
        file_set files;

        debug() << "Generating file list in " << m_source_directory << "\n";

        ambrosia::recursive_scan_directory( insert_iterator<file_set>(files, files.begin()),
                           m_source_directory );

        debug() << "Found " << files.size() << " files in "
                  << m_source_directory << "\n";

        return files;
    }
} // namespace ambrosia
