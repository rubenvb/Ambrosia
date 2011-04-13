/**
  * builder.h
  * Class that executes the build commands (in parallel).
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef BUILDER_H
#define BUILDER_H

// global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/state.h"
#include "Ambrosia/build_config.h"

ambrosia_namespace_begin

class builder : public lib::state
{
public:
    builder( state* parent = 0 );
    ~builder();
};

ambrosia_namespace_end

#endif // BUILDER_H
