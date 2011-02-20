/**
  * BuildConfig.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "BuildConfig.h"

// libAmbrosia includes
/* "Ambrosia/Typedefs.h" */

// C++ includes
/* <string> */
    using std::string;

namespace ambrosia
{
    BuildConfig::BuildConfig()
    :   m_cross_prefix( string() ),
        m_user_options( string_map() ),
        m_target_config( string_map() )
    {   }
} // namespace ambrosia
