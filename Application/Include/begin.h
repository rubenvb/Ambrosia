/**
  * begin.h
  * First State that processes commandline arguments and finds nectar.txt file.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef BEGIN_H
#define BEGIN_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/state.h"
#include "Ambrosia/build_config.h"

// C++ includes
#include <string>

namespace ambrosia
{
    class begin : public state
    {
    public:
        begin( const int argc, const char* const argv[], state* parent = 0 );
        ~begin();

        state* event();

    private:
        bool m_first_dashless_argument; // true if no arguments without '-' was processed
        std::vector<std::string> m_arguments;
        // internal functions
        const std::string find_project_file( const std::string &path );
        bool add_build_target( const std::string &target );
        void print_help_information();
        void print_version_information();
        // TODO: implement internal Ambrosia options
        void set_internal_option( const std::string &option, const std::string &value );
        bool add_configuration_options( const std::string &options );
    };
} // namespace ambrosia

#endif // BEGIN_H
