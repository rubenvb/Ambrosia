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
#include "typedefs.h"

// C++ includes
#include <algorithm>
#include <sstream>

libambrosia_namespace_begin

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

// conversion to string
template <class T>
inline std::string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

libambrosia_namespace_end

#endif // ALGORITHM_H
