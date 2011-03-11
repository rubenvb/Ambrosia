/**
  * build_config.h
  * Information required to set up a chain of build commands from the Target list.
  *  It contains the following:
  *   - cross-compile prefix,
  *   - targets to be built (will be resolved for dependencies later)
  *   - target-specific configurations (release/debug/...)
  *   - user options (double dash '--' options
  *   -...
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iterator>
    using std::insert_iterator;
/* <map> */
/* <string> */
/* <utility> */

namespace ambrosia
{
    class build_config
    {
    public:
        build_config();

    /*
     * Setters
     ***************/
        void set_source_directory( const std::string &source_directory );
        void set_project_file( const std::string &project_file );
        // set cross-compilation options
        void set_ambrosia_cross( const std::string &cross );
        void set_gnu_prefix( const std::string &prefix ); // handles a lot of standard GU triplets
        // add config options to one target
        void add_target_config( const std::string &target, const string_set &options );
        // add config options to all present targets
        void add_general_config( const string_set &options );

    /*
     * Getters
     **********/
        const std::string &source_directory() const;
        const std::string &project_file() const;
        const std::string path_to_project_file() const;

    private:
        std::string m_source_directory;
        std::string m_project_file;
        os m_target_os;
        architecture m_target_architecture;
        toolchain m_target_toolchain;
        std::string m_gnu_prefix; // GNU triplet for platform
        string_map m_user_options; // user option --> value
        map_const_string_set_string m_target_config; // Target name --> optional config list
        // internal functions
        static void merge_options( std::pair<const std::string, string_set> &old_options, const string_set &new_options );
    };
} // namespace ambrosia

#endif // BUILDCONFIG_H
