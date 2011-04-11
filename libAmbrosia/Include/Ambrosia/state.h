/**
  * Ambrosia/state.h
  * Abstract base class for all States of Ambrosia.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef STATE_H
#define STATE_H

// Global include
#include "global.h"

libambrosia_namespace_begin

class state
{
public:
    state( state* parent = 0 );
    state( const state &s );
    virtual ~state();
    state & operator=( const state &s );

    virtual bool end() const;
    virtual state* end_of_state();

    virtual state* event() = 0;

    // Setters
    void set_parent( state* parent = 0 );

private:
    state* p_parent;
};

libambrosia_namespace_end

#endif // STATE_H
