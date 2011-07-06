/**
  * Ambrosia/state.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/state.h"

// libAmbrosia includes
#include "Ambrosia/debug.h"

libambrosia_namespace_begin

state::state( state* parent )
:   p_parent( parent )
{   }
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

libambrosia_namespace_end
