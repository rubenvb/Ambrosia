/**
  * global.h
  * Global header included in all Ambrosia Application headers.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef GLOBAL_APPLICATION_H
#define GLOBAL_APPLICATION_H

// libAmbrosia global include
#include "Ambrosia/global.h"

ambrosia_namespace_begin

namespace app
{
    // Versioning
    const int version_major = 0;
    const int version_minor = 0;
    const int version_bugfix = 1;
}
    using lib::s_build_config;

ambrosia_namespace_end

#endif // GLOBAL_APPLICATION_H
