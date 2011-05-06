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
#include <memory>
/* <set> */
#include <string>

libambrosia_namespace_begin

// forward declarations
class target;

// locates a unique .nectar.txt file in file. More than one emits an error
const std::string find_nectar_file( const std::string &directory );

// converts all project file content to internal representation, adding to the container where "it" outputs to
void drink_nectar( const std::string &filename, target_list &targets );
// uses s_build_config to apply the requested build configuration
void apply_build_config( target_list &targets );

libambrosia_namespace_end

#endif // NECTAR_H
