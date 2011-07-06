/**
  * Ambrosia/Configuration/build_config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Configuration/build_config.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"
#include "Ambrosia/platform.h"
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
:   config(),
    m_user_variables()
{   }

build_config::build_config( const ambrosia_config &config )
{   }

/*
 * Setters
 **********/
bool build_config::add_config( const string &config )
{
    return m_config.insert(config).second;
}
bool build_config::remove_config( const string &config )
{
    return m_config.erase(config);
}

/*
 * Getters
 **********/


libambrosia_namespace_end
