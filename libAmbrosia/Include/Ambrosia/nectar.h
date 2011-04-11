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
#include <string>

libambrosia_namespace_begin

const std::string find_nectar_file( const std::string &directory );

template<class output_iterator>
void drink_nectar( const std::string &filename, output_iterator it );

libambrosia_namespace_end

#endif // NECTAR_H
