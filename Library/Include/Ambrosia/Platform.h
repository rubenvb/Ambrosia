/**
  * Platform.h
  * Ambrosia's platform dependent functions.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PLATFORM_H
#define PLATFORM_H

// Global include
#include "Ambrosia/Global.h"

// libAmbrosia includes
#include "Enums.h"

// C++ include
#include <string>

namespace ambrosia
{
/*
 * constants
 *********************/
    extern const std::string executableSuffix;
    extern const os buildOS;
/*
 * Functions
 ************/
    // current working directory in string form.
    const std::string currentWorkingDir();
    // Recursive directory listing with generic output iterator
    template<class OutIt>
    void scanDir( OutIt it, const std::string &relDir, const std::string &dirName = "" );
}


#endif // PLATFORM_H
