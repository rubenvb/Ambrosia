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
#include "ambrosia_config.h"
#include "debug.h"
#include "file_store.h"
#include "platform.h"
#include "status.h"
#include "target.h"
#include "typedefs.h"
#include "nectar_loader.h"

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
    s_ambrosia_config.set_source_directory( directory );
    file_set candidates = s_file_store.find_source_file( {}, "*.nectar.txt" );
    //scan_directory( std::inserter(file_list, file_list.begin()), directory );
    switch( candidates.size() )
    {
        case 0:
            emit_error( "No *.nectar.txt file found in " + directory );
            break;
        case 1:
            return (*candidates.begin()).first; // first = filename, second = modified
        default:
            emit_error( "Multiple *.nectar.txt files found in directory: " + directory );
            std::for_each( candidates.begin(), candidates.end(),
                       [&candidates](const file &item)
                       {
                           emit_error( "\t" + item.first );
                       } );
    }
    return string("");
}

void drink_nectar( const std::string &filename, target_list &targets )
{
    // open file
    auto stream_ptr( open_ifstream(filename) );
    auto &stream = *stream_ptr;
    if( !stream )
        return emit_error( "Unable to open nectar file: " + filename );

    // read targets
    debug(2) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
    nectar_loader loader( filename, stream );

    loader.extract_nectar( targets );
    if( error_status() )
        return;
}

void apply_build_config( target_list &/*targets*/ )
{
    emit_error( "nectar::apply_build_confi::Not implemented." );
}

libambrosia_namespace_end
