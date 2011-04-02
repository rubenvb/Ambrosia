/**
  * Ambrosia/state.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "state.h"

// libAmbrosia includes
#include "debug.h"

namespace ambrosia
{
    state::state( state* parent )
    :   p_parent( parent )
    {
        debug() << "State created.\n";
    }
    state::state( const state &state )
    :   p_parent( state.p_parent )
    {   }
    state::~state()
    {   }
    state & state::operator=( const state &state )
    {
        p_parent = state.p_parent;
        return *this;
    }

    bool state::end() const
    {
        return false;
    }
    state* state::end_of_state()
    {
        state* parent = p_parent; // save pointer to parent
        delete this;
        return parent;
    }
    void state::set_parent( state* parent )
    {
        p_parent = parent;
    }
}
