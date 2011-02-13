/**
  * State.h
  * Abstract base class for all States of Ambrosia.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef STATE_H
#define STATE_H

// Global include
#include "Ambrosia/Global.h"

namespace ambrosia
{
    class State
    {
    public:
        State( State* parent = 0 );
        State( const State &state );
        virtual ~State();
        State & operator=( const State &state );

        virtual bool end() const;
        virtual State* endOfState();

        virtual State* event() = 0;

        // Setters
        void setParent( State* parent = 0 );

    private:
        State* p_parent;
    };
} // namespace ambrosia

#endif // STATE_H
