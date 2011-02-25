/**
  * Algorithm.h
  * Ambrosia's handy semi-generic algorithms.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ALGORITHM_H
#define ALGORITHM_H

// Global include
#include "Ambrosia/Global.h"

// libAmbrosia includes
#include "Ambrosia/Platform.h"
#include "Ambrosia/Typedefs.h"

namespace ambrosia
{
    // returns true if expanding '?' or '*' between forward slashes '/' produces a match
    bool wildcard_directory_compare( const std::string &wildcard_string, const std::string &full_string );
}

#endif // ALGORITHM_H
