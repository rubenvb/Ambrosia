/**
  * Ambrosia/build_config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "build_config.h"

// libAmbrosia includes
#include "algorithm.h"
#include "enum_maps.h"
#include "debug.h"
#include "status.h"
#include "platform.h"
/* "typedefs.h" */

// C++ includes
#include <iterator>
    using std::insert_iterator;
#include <algorithm>
/* <map> */
    using std::map;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;

libambrosia_namespace_begin

build_config::build_config()
:   m_config(),
    m_user_variables()
{   }

libambrosia_namespace_end
