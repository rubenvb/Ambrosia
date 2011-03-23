/**
  * nectar_loader.h
  * Class to handle read and disect Ambrosia's project files
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
    public:
        nectar_loader( std::ifstream &stream );

        // sort the targets in correct build order
        void dependency_sort();

    private:
        std::ifstream &m_stream;
    };
} // namespace ambrosia

#endif // NECTAR_LOADER_H
