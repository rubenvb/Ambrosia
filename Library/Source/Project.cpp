/**
  * Project.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// class include
#include "Project.h"

// libAmbrosia includes
/* "Target.h" */

// C++ includes
/* <vector> */
    using std::vector;

namespace ambrosia
{
    Project::Project()
    :   m_targets( vector<Target>() )
    {   }
}
