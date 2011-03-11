/**
  * nectar.h
  * Functions to handle Ambrosia's project files.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef NECTAR_LOADER_H
#define NECTAR_LOADER_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "target.h"

// C++ includes
#include <fstream>
#include <string>
#include <vector>

namespace ambrosia
{
    class nectar_loader
    {
        nectar_loader( const std::string &filename );

    public:
        // read the nectar.txt files and store all seperate targets.
        template<class output_iterator>
        void load_nectar( const std::string &filename, output_iterator &it );
        void dependency_sort();

    private:
        std::ifstream m_fstream;
    };
} // namespace ambrosia

#endif // NECTAR_LOADER_H
