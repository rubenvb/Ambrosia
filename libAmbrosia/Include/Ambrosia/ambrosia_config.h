/**
  * Ambrosia/ambrosia_config.h
  * Ambrosia's runtime configuration, used as a basis for command generation and project configuration.
  *  This includes:
  *   - build and target platform and toolchain detection.
  *   - initial CONFIG listing.
  *   - ... (anything
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_CONFIG_H
#define AMBROSIA_CONFIG_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "enums.h"
#include "typedefs.h"

// C++ includes
/* <string> */

libambrosia_namespace_begin

class ambrosia_config
{
public:
    ambrosia_config();

    // Setters
    void set_source_directory( const std::string &source_directory );
    void set_project_file( const std::string &project_file );
    // set cross-compilation options
    void set_ambrosia_cross( const std::string &cross );
    void set_gnu_prefix( const std::string &prefix ); // handles a lot of standard GU triplets
    // add target or add additional config options to one target
    void add_target_config( const std::string &target, const string_set &options );
    // add config options to all present targets
    void add_general_config( const string_set &options );
    // add user option settings
    void set_user_option( const std::string &option, const std::string &value );

    // getters;
    const std::string & source_directory() const;
    const std::string & project_file() const;
    const std::string path_to_project_file() const;
    const os & target_os() const;
    const architecture & target_architecture() const;
    const toolchain & target_toolchain() const;
    const map_string_set_string & target_config() const;

    friend class build_config;

private:
    string_set m_config; // mostly platform dependent stuff
    std::string m_source_directory;
    std::string m_project_file;
    std::string m_build_directory; // if source and build dir are equal, this is ./build
    os m_target_os;
    architecture m_target_architecture;
    toolchain m_target_toolchain;
    map_string_set_string m_target_config;

};

libambrosia_namespace_end

#endif // AMBROSIA_CONFIG_H
