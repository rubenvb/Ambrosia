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
        ~Begin();

        State* event();

    private:
        bool m_first_dashless_argument; // true if no arguments without '-' was processed
        std::vector<std::string> m_arguments;
        // internal functions
        const std::string find_project_file( const std::string &path );
    };
} // namespace ambrosia

#endif // BEGIN_H
