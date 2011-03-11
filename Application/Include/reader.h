/**
  * reader.h
  * Load the *.nectar.txt file and sort out which targets are to be built.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef READER_H
#define READER_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/state.h"

// C++ includes
#include <fstream>

namespace ambrosia
{
    class reader : public state
    {
    public:
        reader( state* parent = 0 );

        state* event();

    private:
        std::ifstream m_stream;
    };
} // namespace

#endif // READER_H
