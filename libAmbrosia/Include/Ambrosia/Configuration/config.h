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
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

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
    bool set_source_directory( const std::string &source_directory );
    void set_project_file( const std::string &project_file );
    bool add_config( const std::string &config );
    bool remove_config( const std::string &config );
    // add user option settings
    void set_user_option( const std::string &option, const std::string &value );


    /*
     * Getters
     **********/
    const std::string & source_directory() const;
    const std::string & project_file() const;
    const std::string path_to_project_file() const;
    const std::string & build_directory() const;
    const os & target_os() const;
    const architecture & target_architecture() const;
    const toolchain & target_toolchain() const;

protected:
    os m_target_os;
    architecture m_target_architecture;
    toolchain m_target_toolchain;

private:
    string_set m_config; // mostly platform dependent stuff
    std::string m_source_directory;
    std::string m_project_file;
    std::string m_build_directory; // if source and build dir are equal, this is ./build
};

libambrosia_namespace_end

#endif // CONFIGURATION_CONFIG_H
