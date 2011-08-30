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
#include "Ambrosia/Configuration/build_config.h"

// Ambrosia includes
#include "state.h"

ambrosia_namespace_begin

class builder : public state
{
public:
    builder( state* parent = 0 );
    ~builder();
};

ambrosia_namespace_end

#endif // BUILDER_H
