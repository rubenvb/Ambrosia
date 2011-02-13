/**
  * State.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "State.h"

namespace ambrosia
{
    State::State( State* parent )
    :   p_parent( parent )
    {   }
    State::State( const State &state )
    :   p_parent( state.p_parent )
    {   }
    State::~State()
    {   }
    State & State::operator=( const State &state )
    {
        p_parent = state.p_parent;
        return *this;
    }

    bool State::end() const
    {
        return false;
    }
    State* State::endOfState()
    {
        State* parent = p_parent; // save pointer to parent
        delete this;
        return parent;
    }
    void State::setParent( State* parent )
    {
        p_parent = parent;
    }
}
