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
#include "Ambrosia/file_store.h"
#include "Ambrosia/status.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

class project
{
public:
    project();

    // commandline
    void apply_commandline_options( string_vector &arguments);

    // nectar
    void find_project_file();
    void drink_nectar();

    // Getters
    const ambrosia_config & config() const;
    ambrosia_config & config();
    // Setters


private:
    static ambrosia_config m_configuration;
    static file_store m_file_store;
    project_list m_subprojects;
};

libambrosia_namespace_end

#endif // AMBROSIA_PROJECT_H
