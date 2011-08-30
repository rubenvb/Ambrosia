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
#include "Ambrosia/debug.h"
    using libambrosia::debug;
/* "Ambrosia/state.h" */
    using ambrosia::state;
#include "Ambrosia/status.h"

// C++ includes
#include <iostream>
    using std::cerr;
    using std::endl;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;

ambrosia_namespace_begin

end_state::end_state( state *parent )
:   state( parent )
{
    debug(0) << "end_state::end_state created.\n";
    lib::print_errors();
}

end_state::end_state( const string &message, state* parent )
:   state( parent )
{
    debug(0) << "end_state::end_state created.\n";
    cerr << message << endl;
}
end_state::end_state( const string &message, const string_vector &list,
          state* parent )
:   state( parent )
{
    debug(0) << "end_state::end_state created.\n";
    cerr << "end_state::" << message << "\n";
    const auto end = list.end();
    for( auto it = list.begin(); it != end; ++it )
    {
        cerr << " " << *it;
    }
}

end_state::~end_state()
{
    debug(0) << "end_state::Destroyed.\n";
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

ambrosia_namespace_end
