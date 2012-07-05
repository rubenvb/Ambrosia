/**
  * Ambrosia/nectar.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/nectar.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"
#include "Ambrosia/typedefs.h"
#include "Ambrosia/nectar_loader.h"

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

void drink_nectar( const std::string& filename, target_vector& targets )
{
    // open file
    auto stream_ptr( open_ifstream(filename) );
    auto& stream = *stream_ptr;
    if( !stream )
        return emit_error( "Unable to open nectar file: " + filename );

    // read targets
    debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
    nectar_loader loader( filename, "", stream );

    loader.extract_nectar( targets );
}

void apply_build_config( target_vector& /*targets*/ )
{
    emit_error( "nectar::apply_build_config::Not implemented." );
}

libambrosia_namespace_end
