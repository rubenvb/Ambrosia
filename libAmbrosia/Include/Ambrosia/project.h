/**
  * Ambrosia/project.h
  * Convenienve class containing everything Ambrosia needs and does.
  *  Everything that uses Ambrosia (plugins, etc...) should go through
  *  here, if possible.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_PROJECT_H
#define AMBROSIA_PROJECT_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/status.h"
#include "Ambrosia/target.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class file_cache;

class project
{
public:
    project( file_cache &file_cache );
    project( ambrosia_config &ambrosia_config, file_cache &file_cache );

    static void set_internal_option( const std::string &option, const std::string &value );

    // read project file(s)
    void read_project_files();

    static ambrosia_config* configuration;

private:
    file_cache &m_file_cache;
    target_list m_targets;
    project_list m_subprojects;
};

libambrosia_namespace_end

#endif // AMBROSIA_PROJECT_H
