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

// C++ includes
#include <algorithm>
#include <memory>
#include <sstream>

libambrosia_namespace_begin

/* Freestanding functions (libAmbrosia independent, including no untransparent error handling)
 *************************/
// returns true if expanding '?' or '*' produces a match
bool wildcard_compare( const std::string &wildcard_string, const std::string &full_string );
// returns true if expanding '?' or '*' between forward slashes '/' produces a match
bool wildcard_directory_compare( const std::string &wildcard_string, const std::string &full_string );
// puts whitespace around every occurrence of the char's in the set<char>
const std::string tokenize( const std::string &line, const std::set<char> &special_characters );
// returns true if container contains element
template <class container>
bool contains(const container &cont, const typename container::value_type & elem)
{
   return( std::find(cont.begin(), cont.end(), elem) != cont.end() );
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
// Dependency resolving sort
void dependency_sort( std::vector<target> &targets );

libambrosia_namespace_end

#endif // ALGORITHM_H
