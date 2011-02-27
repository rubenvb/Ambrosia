/**
  * end_state.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "end_state.h"

// libAmbrosia includes
#include "Ambrosia/Debug.h"
#include "Ambrosia/Error.h"

// C++ includes
#include <iostream>
    using std::cerr;
    using std::endl;
/* <string> */
    using std::string;

namespace ambrosia
{
    end_state::end_state( state *parent )
    :   state( parent )
    {
        cerr << "Error: ";
        error::print_errors();
    }

    end_state::end_state( const string &message, state* parent )
    :   state( parent )
    {
        debug() << "end::end created.\n";
        cerr << message << endl;
    }
    end_state::end_state( const string &message, const string_vector &list,
              state* parent )
    :   state( parent )
    {
        cerr << "end_state::" << message << "\n";
        const auto end = list.end();
        for( auto it = list.begin(); it != end; ++it )
        {
            cerr << " " << *it;
        }
    }

    end_state::~end_state()
    {
        debug() << "end_state::Destroyed.\n";
    }

    bool end_state::end() const
    {
        return true;
    }
    state* end_state::event()
    {
        cerr << "end's event() shouldn't be called..." << endl;
        return end_of_state();
    }
} // namespace ambrosia
