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
/* "build_config.h" */
#include "enums.h"
#include "nectar.h"
#include "node.h"
/* "typedefs.h" */

// C++ includes
/* <string> */
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

// temporary variable before all targets have their own build_config member with the
static map_string_set_string s_target_config;

class target : public node
{
public:
    target( const std::string &name, const target_type type,
            const dependency_list &dependencies, const build_config &config = s_ambrosia_config );

    // Getters
    const std::string & name() const;
    target_type type() const;
    const dependency_list & dependencies() const;
    const std::string &filename() const;

    // Setters
    bool add_config( const std::string &config );
    bool remove_config( const std::string &config );
    bool add_file( const file_type type, const std::string &filename );
    bool remove_file( const file_type type, const std::string &filename );
    bool add_directory( const file_type type, const std::string &directory );
    bool remove_directory( const file_type type, const std::string &directory );
    void set_output_name( const std::string &name );

private:
    const target_type m_type; // target type
    const dependency_list m_dependencies; // dependency+type
    build_config m_build_config; // build configuration, inherited from s_ambrosia_config
    map_file_type_string_set m_source_directories; // source directories per file type
    map_file_type_file_set m_source_files; // source files per file type with last modified time
    std::string m_output_name;
};

libambrosia_namespace_end

#endif // TARGET_H
