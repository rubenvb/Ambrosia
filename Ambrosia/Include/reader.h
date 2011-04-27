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
#include "Ambrosia/target.h"

// C++ includes
#include <fstream>
#include <memory>
#include <vector>

ambrosia_namespace_begin

class reader : public lib::state
{
public:
    reader( state* parent = 0 );
    ~reader();

    state* event();

private:
    std::ifstream m_stream;
    std::vector<lib::target> m_targets;
};

ambrosia_namespace_end

#endif // READER_H
