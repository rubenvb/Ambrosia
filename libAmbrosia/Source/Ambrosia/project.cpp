/**
  * Ambrosia/project.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

#include "Ambrosia/project.h"

libambrosia_namespace_begin

ambrosia_config project::m_configuration = ambrosia_config();
file_store project::m_file_store = file_store();

project::project()
:   m_subprojects()
{   }

// Getters
const ambrosia_config & project::config() const
{
    return m_configuration;
}
ambrosia_config & project::config()
{
    return m_configuration;
}

// Setters

libambrosia_namespace_end
