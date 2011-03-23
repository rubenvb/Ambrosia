/**
  * nectar.h
  * Functions to facilitate working with *.nectar.txt files
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef NECTAR_H
#define NECTAR_H

// Global include
#include "Ambrosia/global.h"

// C++ includes
#include <string>

namespace ambrosia
{
    const std::string find_nectar_file( const std::string &directory );

    template<class output_iterator>
    void drink_nectar( const std::string &filename, output_iterator it );
} // namespace ambrosia

#endif // NECTAR_H
