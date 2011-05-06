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
#include "algorithm.h"
#include "status.h"
#include "debug.h"
#include "platform.h"
#include "target.h"
#include "typedefs.h"
    using ambrosia::string_vector;
#include "Parser/nectar_loader.h"

// C++ includes
#include <fstream>
    using std::ifstream;
#include <iterator>
    using std::back_insert_iterator;
#include <memory>
    using std::unique_ptr;
/* <set> */
    using std::set;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

// C-ish includes
#include <ctime>

libambrosia_namespace_begin

const string find_nectar_file( const string &directory )
{
    debug() << "nectar::find_nectar_file called for: " << directory << ".\n";
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
        emit_error( "More than one *.nectar.txt files found in specified directory: " + directory + ".\n"
                    + "Ambrosia cannot decide which project file you want to use.\n"
                    + "Specify a file on the commandline: \"Ambrosia path/to/project.nectar.txt\"." );
    // If there are no or more than one match, return an empty string
    return string();
}

void drink_nectar( const std::string &filename, target_list &targets )
{
    // open file
    ifstream stream( filename );
    if( !stream )
        return emit_error( "Unable to open nectar file: " + filename );

    // read targets
    debug(2) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
    nectar_loader loader( filename, stream );
    loader.extract_nectar( targets );
    if( warning_status() )
        print_warnings();
    if( error_status() )
        return;
}

void apply_build_config( target_list &targets )
{

}

libambrosia_namespace_end
