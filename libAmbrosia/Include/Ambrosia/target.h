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

// C++ includes
/* <string> */
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

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
    void add_config( const std::string &config ); // forwards to m_build_config::add_config
    void add_file( const file_type, const std::string &file );
    void add_files( const file_type, const std::set<std::string> &files );


private:
    const target_type m_type; // target type
    const dependency_list m_dependencies; // dependency+type
    const build_config m_build_config;
};

libambrosia_namespace_end

#endif // TARGET_H
