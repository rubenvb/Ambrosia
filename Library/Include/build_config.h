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
#include "Ambrosia/typedefs.h"

namespace ambrosia
{
    class build_config
    {
    public:
        build_config();

    private:
        std::string m_cross_prefix; // GNU triplet for platform
        string_map m_user_options; // user option --> value
        string_map m_target_config; // Target name --> config list
    };
} // namespace ambrosia

#endif // BUILDCONFIG_H
