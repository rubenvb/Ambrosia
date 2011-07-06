/**
  * Ambrosia/Configuration/config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Configuration/config.h"

// libambrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/platform.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

config::config()
:   m_config(),
    m_source_directory(),
    m_project_file(),
    m_build_directory(),
    m_target_os( build_os ),
    m_target_architecture( build_architecture ),
    m_target_toolchain( toolchain::GNU )
{   }
config::~config()
{   }

/*
 * Getters
 **********/
const string & config::source_directory() const
{
    return m_source_directory;
}
const string & config::project_file() const
{
    return m_project_file;
}
const std::string config::path_to_project_file() const
{
    return m_source_directory + "/" + m_project_file;
}
const string & config::build_directory() const
{
    return m_build_directory;
}
const os & config::target_os() const
{
    return m_target_os;
}
const architecture & config::target_architecture() const
{
    return m_target_architecture;
}
const toolchain & config::target_toolchain() const
{
    return m_target_toolchain;
}


libambrosia_namespace_end
