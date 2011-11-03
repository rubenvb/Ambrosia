/**
  * Ambrosia/config_baseuration/config_base.h
  * Abstract class for storing config_baseuration options.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

class config_base
{
public:
    config_base();
    config_base( toolchain requested_toolchain );
    virtual ~config_base() = 0; // abstractify class

    /*
     * Setters
     **********/
    // source directory and file where the object's config_base is attached to
    bool set_source_directory( const std::string &source_directory );
    void set_project_file( const std::string &project_file );
    bool add_config( const std::string &config );
    bool remove_config( const std::string &config );
    // add user option settings
    void set_user_option( const std::string &option, const std::string &value );

    /*
     * Getters
     **********/
    string_set & config();
    const string_set & config() const;
    const std::string & source_directory() const;
    const std::string & project_file() const;
    const std::string path_to_project_file() const;
    const std::string & build_directory() const;
    const os & target_os() const;
    const architecture & target_architecture() const;
    const toolchain & target_toolchain() const;

protected:
    // Environment PATH
    const string_vector m_environment_PATH;
    // build platform identification
    architecture m_build_architecture;
    environment m_build_environment;
    os m_build_os;
    toolchain m_build_toolchain; // useful for building tools used in the build

    // target platform identification
    architecture m_target_architecture;
    os m_target_os;
    toolchain m_target_toolchain;

    std::string m_source_directory;

private:
    string_set m_config; // mostly platform dependent stuff
    std::string m_project_file;
    std::string m_build_directory; // if source and build dir are equal, this is ./build
    // Platform detection functions
    architecture detect_build_architecture() const;
    environment detect_build_environment() const;
    toolchain detect_toolchain() const;
    toolchain detect_toolchain( toolchain requested_toolchain ) const;
    void initialize_config();
};

libambrosia_namespace_end

#endif // CONFIG_BASE_H
