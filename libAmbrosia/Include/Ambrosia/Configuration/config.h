/**
  * Ambrosia/Configuration/config.h
  * Abstract class for storing configuration options.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef CONFIGURATION_CONFIG_H
#define CONFIGURATION_CONFIG_H

// Global include
#include <global.h>

// libAmbrosia includes
#include "enums.h"
#include "typedefs.h"

libambrosia_namespace_begin

class config
{
public:
    config();
    virtual ~config() = 0; // abstractify class

    /*
     * Setters
     **********/
    // source directory and file where the object's config is attached to
    void set_source_directory( const std::string &source_directory );
    void set_project_file( const std::string &project_file );
    // add user option settings
    void set_user_option( const std::string &option, const std::string &value );

protected:
    string_set m_config; // mostly platform dependent stuff
    std::string m_source_directory;
    std::string m_project_file;
    std::string m_build_directory; // if source and build dir are equal, this is ./build
    os m_target_os;
    architecture m_target_architecture;
    toolchain m_target_toolchain;
};

libambrosia_namespace_end

#endif // CONFIGURATION_CONFIG_H
