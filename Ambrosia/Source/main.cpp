/**
  * main.cpp
  * Ambrosia's main function.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Ambrosia includes
#include "commandline.h"
#include "help_and_version_output.h"
using namespace ambrosia;

// libAmbrosia includes
#include "Ambrosia/Error/error.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/project.h"

// C-ish includes
#include <cstdlib>
#include <ctime>

// C++ includes
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main( int argc, char* argv[] )
try {
    // Welcome message
    print_version_information();

    lib::file_cache file_cache;
    lib::project project( file_cache );

    apply_commandline_options( string_vector(argv+1, argv+argc), project,
                               file_cache );

    project.read_project_files();

    /*state* current_state = new ambrosia::begin( argc, argv );
    // Main event loop
    while( !current_state->end() )
    {
        current_state = current_state->event();
    }
    //const double exec_time = difftime( time(0), t );
    //t = time(0);
    // Cleanup all remaining states in the chain
    //debug(0) << "main::State cleanup...\n";
    while( current_state != NULL )
    {
        current_state = current_state->end_of_state();
    }
    //const double cleanup_time = difftime( t, time(0) );

    //debug(0) << "main::Execution took " << exec_time << " seconds.\n"
    //     << "Cleanup took " << cleanup_time << " seconds.\n";
#ifdef AMBROSIA_DEBUG
    //debug(0) << "error_status() was called " << s_error_status_calls << " times.\n"
    //     << "full_directory_name() was called " << s_full_directory_name_calls << " times.\n";
#endif*/
}
catch( libambrosia::error &e )
{
    e.output_message();
}
catch( std::exception &e )
{
    cout << "something bad happened:\n";
    cout << e.what();
}
