/**
  * Main.cpp
  * Ambrosia's main function.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Ambrosia includes
#include "Begin.h"
#include "Ambrosia/Debug.h"

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

int main( int argc, char* argv[] )
{
    time_t t = time(0);

    ambrosia::State* state = new ambrosia::Begin( argc, argv );
    // Main event loop
    while( !state->end() )
    {
        state = state->event();
    }
    const double exectime = difftime( time(0), t );
    t = time(0);
    // Cleanup all remaining states in the chain
    ambrosia::Debug() << "State cleanup...\n";
    while( state != NULL )
    {
        state = state->endOfState();
    }
    const double cleanuptime = difftime( t, time(0) );

    ambrosia::Debug() << "Execution took " << exectime << " seconds.\n"
         << "Cleanup took " << cleanuptime << " seconds.\n";

    return 0;
}
