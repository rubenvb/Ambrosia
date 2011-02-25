/**
  * End.h
  * Error handler state, which ends the main event loop at any time.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef END_H
#define END_H

// Global include
#include "Global.h"

// libAmbrosia includes
#include "Ambrosia/Typedefs.h"
#include "Ambrosia/State.h"

// C++ includes
/* <string> */

namespace ambrosia
{
    class End : public State
    {
    public:
        // print "Error:" followed by libAmbrosia's error::print_errors
        End( State* parent = 0 );
        // print contents of "message"
        End( const std::string &message, State* parent = 0 );
        // pring contents of "message", followed by every item in "list"
        End( const std::string &message, const string_vector &list,
             State* parent = 0 );
        ~End();

        bool end() const;
        State* event();
    };
} // namespace ambrosia

#endif // END_H
