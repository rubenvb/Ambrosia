/**
  * build_config.h
  * Information required to set up a chain of build commands from the Target list.
  *  It contains the following:
  *   - cross-compile prefix,
  *   - targets to be built (will be resolved for dependencies later)
  *   - target-specific configurations (release/debug/...)
  *   - user options (double dash '--' options
  *   -...
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
/* <map> */
/* <string> */

namespace ambrosia
{
    class build_config
    {
    public:
        build_config();

        // set cross-compilation options
        void set_ambrosia_cross( const std::string &cross );
        void set_gnu_prefix( const std::string &prefix ); // handles a lot of standard GU triplets
        // add config value to one target
        void add_target_config( const std::string &target, const std::string &config );

    private:
        os m_target_os;
        architecture m_target_architecture;
        std::string m_gnu_prefix; // GNU triplet for platform
        string_map m_user_options; // user option --> value
        string_map m_target_config; // Target name --> config list
        const static std::map<std::string, os> s_os_map;
        const static std::map<std::string, architecture> s_architecture_map;
        const static std::map<std::string, toolchain> s_toolchain_map;
    };
} // namespace ambrosia

#endif // BUILDCONFIG_H
