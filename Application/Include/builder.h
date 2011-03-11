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

namespace ambrosia
{
    class builder : public state
    {
    public:
        builder( state* parent = 0 );

        static build_config s_build_config;
    };
} // namespace ambrosia

#endif // BUILDER_H
