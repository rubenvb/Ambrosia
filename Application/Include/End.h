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
        End( State* parent = 0 );
        End( const std::string &message, State* parent = 0 );
        End( const std::string &message, const ambrosia::stringList &list,
             State* parent = 0 );
        ~End();

        bool end() const;
        State* event();
    };
} // namespace ambrosia

#endif // END_H
