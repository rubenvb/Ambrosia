/**
  * Ambrosia/target.h
  * Ambrosia's internal representation of a build target. This includes:
  *  - full nectar.txt target text for:
  *     > multiple builds in one
  *     > decent error reporting referring to line number and text
  *  - full source file list (including headers etc.)
  *  - inter-target dependencies
  *  - ...

  * Author: Ruben Van Boxem
  *
  **/

#ifndef TARGET_H
#define TARGET_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/Configuration/build_config.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/node.h"
#include "Ambrosia/typedefs.h"

// C++ includes
/* <string> */
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

class target : public node
{
public:
    // 'global' target with subproject-wide dependencies
    target( const std::string &subdirectory,
            const dependency_list &dependencies, const ambrosia_config &config );
    // other targets are based off of global
    target( const std::string &name, const target_type type,
            const dependency_list &dependencies, const build_config &config );

    // Getters
    const std::string & name() const;
    target_type type() const;
    const dependency_list & dependencies() const;
    const std::string &filename() const;

    // Setters
    build_config & config();
    const build_config &config() const;
    void add_source_file( const file_type type, const std::string &filename,
                          const std::string &nectar_file, const size_t line_number );
    void remove_file( const file_type type, const std::string &filename );
    bool add_source_directory( const file_type type, const std::string &directory );
    void remove_directory( const file_type type, const std::string &directory );
    void set_output_name( const std::string &name );

private:
    const target_type m_type; // target type
    const dependency_list m_dependencies; // dependency+type
    build_config m_build_config; // build configuration, inherited from global target's build_config
    map_file_type_string_set m_source_directories; // source directories per file type
    map_file_type_file_set m_source_files; // source files per file type with last modified time
    std::string m_output_name;
};

libambrosia_namespace_end

#endif // TARGET_H
