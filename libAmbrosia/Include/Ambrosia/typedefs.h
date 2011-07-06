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
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"

// C++ includes
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

/*
 * STL typedefs
 ***************/

ambrosia_namespace_begin

    // Homogeneous typedefs
   typedef std::pair<std::string, std::string> string_pair;
   typedef std::map<std::string, std::string> string_map;
   typedef std::vector<std::string> string_vector;
   typedef std::set<std::string> string_set;

    // Heterogeneous typedefs
    typedef std::pair<std::string, bool> pair_string_bool;
    typedef std::pair<std::string, std::set<std::string> > pair_string_string_set;
    typedef std::map<std::string, std::set<std::string> > map_string_set_string;
    typedef std::pair<std::string, time_t> file;
    typedef std::map<std::string, time_t> file_set;

ambrosia_namespace_end

/*
 * libAmbrosia typedefs
 ***********************/

libambrosia_namespace_begin

    // Forward declarations
    class target;

    // homogeneous typedefs
    typedef std::unique_ptr<target> target_ptr;
    typedef std::vector<target_ptr> target_list;

    // heterogeneous typedefs
    typedef std::set<std::pair<target_type, std::string> > dependency_list;
    typedef std::map<std::string, file_set> map_string_file_set;
    typedef std::map<file_type, string_set> map_file_type_string_set;
    typedef std::map<file_type, file_set> map_file_type_file_set;

libambrosia_namespace_end

#endif // TYPEDEFS_H
