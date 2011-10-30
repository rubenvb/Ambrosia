/**
  * main.cpp
  * Ambrosia's main function.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Ambrosia includes
#include "begin.h"

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
    time_t t = time(0);

    state* current_state = new ambrosia::begin( argc, argv );
    // Main event loop
    while( !current_state->end() )
    {
        current_state = current_state->event();
    }
    const double exec_time = difftime( time(0), t );
    t = time(0);
    // Cleanup all remaining states in the chain
    debug(0) << "main::State cleanup...\n";
    while( current_state != NULL )
    {
        current_state = current_state->end_of_state();
    }
    const double cleanup_time = difftime( t, time(0) );

    debug(0) << "main::Execution took " << exec_time << " seconds.\n"
         << "Cleanup took " << cleanup_time << " seconds.\n"
         << "error_status() was called " << error_status_calls << " times.\n";

    return 0;
}
