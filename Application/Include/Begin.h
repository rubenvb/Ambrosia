/**
  * Begin.h
  * First State that processes commandline arguments and finds nectar.txt file.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef BEGIN_H
#define BEGIN_H

// Global include
#include "Global.h"

// libAmbrosia includes
#include "Ambrosia/State.h"

// C++ includes
#include <string>

namespace ambrosia
{
    class Begin : public State
    {
    public:
        Begin( const int argc, const char* const argv[], State* parent = 0 );

        State* event();

    private:
        std::string m_arguments;

    };
} // namespace ambrosia

#endif // BEGIN_H
