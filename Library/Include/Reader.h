/**
  * Reader.h
  * Ambrosia's file reader. Seperates the nectar files into targets
  *  and sets up dependencies.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef READER_H
#define READER_H

// Global include
#include "Ambrosia/Global.h"

// C++ includes
#include <string>
#include <vector>

namespace ambrosia
{
    class Reader
    {
    public:
        Reader();

        // read the nectar.txt files and store all seperate targets.
        void load_nectar( const std::string &filename );
        void dependency_sort();

    private:
        std::vector<std::string> m_targets;
    };
} // namespace ambrosia

#endif // READER_H
