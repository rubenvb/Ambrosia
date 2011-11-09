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
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iterator>
#include <memory>
/* <set> */
#include <string>

libambrosia_namespace_begin

// forward declarations
class ambrosia_config;
class target;

// locates a unique .nectar.txt file in file. More than one emits an error
const std::string find_nectar_file( const std::string &directory );
// finds and sets project file
bool find_project_file( const std::string &path, ambrosia_config &config );

// converts all project file content to internal representation, adding to the container where "it" outputs to
void drink_nectar( const std::string &filename, target_list &targets );
// uses s_ambrosia_config to apply the requested build configuration
void apply_build_config( target_list &targets );

libambrosia_namespace_end

#endif // NECTAR_H
