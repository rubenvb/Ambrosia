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
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

build_config::build_config(const string& subdirectory,
                           const config_base& config)
: config_base(config),
  m_source_types(),
  m_user_variables()
{
  m_source_directory = full_directory_name( m_source_directory, subdirectory );
}

/*
 * Setters
 **********/


/*
 * Getters
 **********/


libambrosia_namespace_end
