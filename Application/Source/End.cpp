/**
  * End.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "End.h"

// libAmbrosia includes
#include "Ambrosia/Debug.h"

// C++ includes
#include <iostream>
    using std::cerr;
    using std::endl;
/* <string> */
    using std::string;

namespace ambrosia
{
    End::End( const string &message, State* parent )
    :   State( parent )
    {
        Debug() << "End created.\n";
        cerr << message << endl;
    }
    End::End( const string &message, const stringList &list,
              State* parent )
    :   State( parent )
    {
        cerr << message << "\n";
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            cerr << " " << *it;
        }
    }

    End::~End()
    {
        Debug() << "End destroyed.\n";
    }

    bool End::end() const
    {
        return true;
    }
    State* End::event()
    {
        cerr << "End's event() shouldn't be called..." << endl;
        return endOfState();
    }
} // namespace ambrosia
