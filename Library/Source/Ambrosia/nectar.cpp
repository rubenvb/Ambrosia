/**
  * Ambrosia/nectar.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "nectar.h"

// libAmbrosia includes
#include "status.h"
#include "debug.h"
#include "platform.h"
#include "target.h"
#include "typedefs.h"
    using ambrosia::string_vector;
#include "nectar_loader.h"

// C++ includes
#include <fstream>
    using std::ifstream;
#include <iterator>
    using std::back_insert_iterator;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

// C-ish includes
#include <ctime>

namespace ambrosia
{
    const string find_nectar_file( const string &directory )
    {
        debug() << "nectar::find_nectar_file called.\n";
        string_vector file_list;
        scan_directory( std::back_inserter(file_list), directory );
        debug() << "nectar::found " << file_list.size() << " files:\n";
        const auto end = file_list.end();
        string_vector nectar_files;
        for( auto it = file_list.begin(); it != end; ++it )
        {
            const string file( *it );
            // find the first *.nectar.txt file in main source directory (not a subdirectory)
            size_t index = file.rfind( ".nectar.txt" );
            if( index < string::npos )
            {
                debug() << "nectar::searching for nectar.txt file... " << file << "\n";
                nectar_files.push_back( file );
            }
        }

        if( nectar_files.size() == 1 )
            return nectar_files[0]; // return the unique match
        else if( !nectar_files.empty() )
            ambrosia::emit_error( "More than one *.nectar.txt files found in specified directory: " + directory + ".\n"
                                  + "Ambrosia cannot decide which project file you want to use.\n"
                                  + "Specify a file on the commandline: \"Ambrosia path/to/project.nectar.txt\"." );
        // If there are no or more than one match, return an empty string
        return string();
    }

    template<class output_iterator>
    void drink_nectar( const std::string &filename, output_iterator it )
    {
        ifstream stream( filename );
        if( !stream )
            ambrosia::emit_error( "Unable to open nectar file: " + filename );

        nectar_loader loader( stream );
        loader.extract_nectar( it );

    }
    // Explicit template instantiation
    template void drink_nectar<back_insert_iterator<vector<target> > >( const string &, back_insert_iterator<vector<target> > );
} // namespace ambrosia
