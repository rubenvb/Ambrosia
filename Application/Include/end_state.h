/**
  * end_state.h
  * Error handler state, which ends the main event loop at any time.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef end_H
#define end_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"
#include "Ambrosia/state.h"

// C++ includes
/* <string> */

namespace ambrosia
{
    class end_state : public state
    {
    public:
        // print "Error:" followed by libAmbrosia's error::print_errors
        end_state( state* parent = 0 );
        // print contents of "message"
        end_state( const std::string &message, state* parent = 0 );
        // pring contents of "message", followed by every item in "list"
        end_state( const std::string &message, const string_vector &list,
             state* parent = 0 );
        ~end_state();

        bool end() const;
        state* event();
    };
} // namespace ambrosia

#endif // END_H
