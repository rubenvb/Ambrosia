/**
  * Ambrosia/build_config.h
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
#include "global.h"

// libAmbrosia includes
#include "enums.h"
#include "typedefs.h"

// C++ includes
/* <map> */
/* <string> */
/* <utility> */

libambrosia_namespace_begin

// Forward declarations
class ambrosia_config;

class build_config
{
public:
    build_config();
    build_config( const ambrosia_config &config ); // conversion constructor

    // Setters
    bool add_config( const std::string &config );
    bool remove_config( const std::string &config );

    //Getters
    bool contains( const std::string &config ) const;
    const string_map & user_options() const;

private:
    string_set m_config; // CONFIG
    string_map m_user_variables; // user string --> value
};

libambrosia_namespace_end

#endif // BUILDCONFIG_H
