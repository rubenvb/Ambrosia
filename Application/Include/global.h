/**
  * global.h
  * Global header included in all Ambrosia Application headers.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef GLOBAL_APPLICATION_H
#define GLOBAL_APPLICATION_H

// libAmbrosia includes
#include "Ambrosia/build_config.h"

namespace ambrosia
{
    // Versioning
    const int app_version_major = 0;
    const int app_version_minor = 0;
    const int app_version_bugfix = 1;

    extern build_config s_build_config;
}

#endif // GLOBAL_APPLICATION_H
