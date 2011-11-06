/**
  * Ambrosia/target_verifier.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "target_verifier.h"

// Ambrosia includes
#include "end_state.h"

ambrosia_namespace_begin

target_verifier::target_verifier( lib::target_list &targets, state* parent )
:   state( parent ),
    m_targets( targets )
{   }

state* target_verifier::event()
{
    return new end_state( "target_verifier does nothing for now." );
}

ambrosia_namespace_end
