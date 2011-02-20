/**
  * Project.h
  * Ambrosia's internal representation of a project. Contains everything
  *  Ambrosia has done:
  *   - Target list
  *   - Configuration information (build/target platform etc.)
  *   - several access functions for basic interfacing with Ambrosia.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PROJECT_H
#define PROJECT_H

// Global includes
#include "Ambrosia/Global.h"

// libAmbrosia includes
#include "Target.h"

// C++ includes
#include <string>
#include <vector>

// Forward declarations
class BuildConfig;

namespace ambrosia
{
    class Project
    {
    public:
        Project();
        Project( const std::string &filename, const BuildConfig &build_config );

    private:
        std::vector<Target> m_targets; // a list of all Targets
    };
} // namespace ambrosia

#endif // PROJECT_H
