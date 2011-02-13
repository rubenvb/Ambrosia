/**
  * Typedefs.h
  * Handy typedefs used throughout Ambrosia code as shorthand notation.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// Global includes
#include "Ambrosia/Global.h"

// C++ includes
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace ambrosia
{
    typedef std::pair<std::string, bool> stringBool;
    typedef std::vector<std::string> stringList;
    typedef std::set<std::string> stringSet;
    typedef std::map<std::string, std::string> stringMap;
    typedef std::map<std::string, time_t> fileSet;
} // namespace ambrosia

#endif // TYPEDEFS_H
