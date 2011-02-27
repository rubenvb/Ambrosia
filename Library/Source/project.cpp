/**
  * project.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// class include
#include "project.h"

// libAmbrosia includes
/* "target.h" */

// C++ includes
/* <vector> */
    using std::vector;

namespace ambrosia
{
    project::project()
    :   m_targets( vector<target>() )
    {   }
}
