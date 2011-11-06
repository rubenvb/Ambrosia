/**
  * Ambrosia/target_verifier.h
  * State to verify all targets and the information contained within.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TARGET_VERIFIER_H
#define TARGET_VERIFIER_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// Ambrosia includes
#include "state.h"

ambrosia_namespace_begin

class target_verifier : public state
{
public:
    target_verifier( lib::target_list &targets, state* parent = 0 );

    state* event();

private:
    lib::target_list &m_targets;
};

ambrosia_namespace_end

#endif // TARGET_VERIFIER_H
