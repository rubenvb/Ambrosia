/**
  * Ambrosia/nectar.h
  * Functions to facilitate working with *.nectar.txt files
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef NECTAR_H
#define NECTAR_H

// Global include
#include "global.h"

// C++ includes
#include <iterator>
/* <set> */
#include <string>

libambrosia_namespace_begin

extern const std::set<char> s_special_characters;
extern const std::set<char>::const_iterator s_special_characters_end;

const std::string find_nectar_file( const std::string &directory );

// converts all project file content to internal representation, adding to the container where "it" outputs to
template<class output_iterator>
void drink_nectar( const std::string &filename, output_iterator it );

libambrosia_namespace_end

#endif // NECTAR_H
