/**
  * Ambrosia/algorithm.h
  * Ambrosia's handy semi-generic algorithms.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ALGORITHM_H
#define ALGORITHM_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "platform.h"
#include "target.h"
#include "typedefs.h"

// C++ includes
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <sstream>

libambrosia_namespace_begin

/* Freestanding functions (libAmbrosia independent, including no untransparent error handling)
 *************************/
// returns true if expanding '?' or '*' produces a match
bool wildcard_compare( const std::string &wildcard_string, const std::string &full_string );
// returns true if expanding '?' or '*' between forward slashes '/' produces a match
bool wildcard_directory_compare( const std::string &wildcard_string, const std::string &full_string );
// compares unfiltered to reference, extracts duplicates and leaves unfiltered orthogonal on reference
void filter_duplicates( std::set<std::string> &unfiltered, const std::set<std::string> &reference,
                        std::set<std::string> &duplicates );
// returns true if container contains element
template <class container>
bool contains(const container &cont, const typename container::value_type &elem )
{
   return( std::find(cont.begin(), cont.end(), elem) != cont.end() );
}
inline bool has_space( const std::string &str )
{
    return contains( str, ' ' );
}
// conversion from string to numeric type
template<class T>
T from_string( const std::string &s )
{
     std::istringstream stream( s );
     T t;
     stream >> t;
     return t;
}
// conversion to string
template <class T>
const std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

/* Ambrosia dependent functions (use one or more of libAmbrosia's functions/classes)
 *******************************/
void skip_BOM( std::istream &stream );
// Dependency resolving algorithm
void dependency_resolve( target_list &unsorted, target_list::iterator node,
                         target_list &resolved, target_list &unresolved );
// Dependency resolving sort
void dependency_sort( target_list &unsorted );
// Dependency resolving sort that filters out targets not present in s_build_config::targets_config
void filter_dependency_sort( target_list &unsorted );
// find files that match the (wildcard) string in a list of files
template<class output_iterator>
void find_matching_files( const std::string &filename, const std::map<std::string, file_set> &directories,
                          output_iterator it );

libambrosia_namespace_end

#endif // ALGORITHM_H
