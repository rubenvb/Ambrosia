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
            const dependency_list &dependencies, const build_config &config = s_build_config );

    // Getters
    const std::string & name() const;
    const target_type & type() const;
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
    const build_config m_build_config;
    directory_map m_directories;
    map_file_type_map_string_file_set m_files_on_disk; // directories with contained files read from disk when added.
};

libambrosia_namespace_end

#endif // TARGET_H
