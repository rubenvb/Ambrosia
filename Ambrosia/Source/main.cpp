/**
  * main.cpp
  * Ambrosia's main function.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Ambrosia includes
#include "begin.h"
#include "commandline.h"
#include "help_and_version_output.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"

// C-ish includes
#include <cstdlib>
#include <ctime>

// C++ includes
#include <iomanip>
#include <iosfwd>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace ambrosia;
using namespace ambrosia::lib;

int main( int argc, char* argv[] )
{
#ifdef AMBROSIA_DEBUG
    //time_t t = time(0);
#endif

    // Welcome message
    print_version_information();

    // create default config
    ambrosia_config global_config;
    // Apply commandline options
    if( !apply_commandline_options(string_vector(argv+1, argv+argc), global_config) )
        return EXIT_SUCCESS; // false return value means execution ending argument like '-v'
    if(error_status())
        return print_errors();


    state* current_state = new ambrosia::begin( argc, argv );
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
#endif
    return 0;
}
