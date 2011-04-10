/**
  * Ambrosia/typedefs.h
  * Handy typedefs used throughout Ambrosia code as shorthand notation.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// Global includes
#include "global.h"

// C++ includes
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

ambrosia_namespace_begin

typedef std::pair<std::string, bool> pair_string_bool;
typedef std::pair<std::string, std::set<std::string> > pair_string_string_set;
typedef std::map<std::string, std::set<std::string> > map_const_string_set_string;
typedef std::vector<std::string> string_vector;
typedef std::set<std::string> string_set;
typedef std::map<std::string, std::string> string_map;
typedef std::map<std::string, time_t> file_set;

ambrosia_namespace_end

#endif // TYPEDEFS_H
