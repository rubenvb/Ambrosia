/**
  * build_config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "build_config.h"

// libAmbrosia includes
/* "Ambrosia/typedefs.h" */

// C++ includes
/* <string> */
    using std::string;

namespace ambrosia
{
    build_config::build_config()
    :   m_cross_prefix( string() ),
        m_user_options( string_map() ),
        m_target_config( string_map() )
    {   }
} // namespace ambrosia
