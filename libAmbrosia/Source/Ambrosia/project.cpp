/**
  * Ambrosia/project.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/project.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
/* <string> */
    using std::string;
#include <fstream>
    using std::ifstream;

libambrosia_namespace_begin

ambrosia_config* project::configuration = NULL;

project::project( ambrosia_config &ambrosia_config, file_cache &file_cache)
:   m_file_cache(file_cache),
    m_targets()
{
    configuration = &ambrosia_config;
}
project::project( file_cache &file_cache)
:   m_file_cache(file_cache),
    m_targets()
{   }

// read project file(s) and store the info and dependencies in target_list
void project::read_project_files()
{
    // open file
    const string &filename = configuration->project_file();
    auto stream_ptr = open_ifstream(filename);
    auto &stream = *stream_ptr;
    if(!stream )
        throw error( "Unable to open *.nectar.txt file: " + filename );

    // read targets
    debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
    nectar_loader loader( filename, "", stream );

    loader.extract_nectar( m_targets );
}

// Filter target_list and sort the targets that need to be built.
void project::apply_target_configuration()
{
    const map_string_set_string target_config = configuration->target_config_options();

    for( auto it = target_config.begin(); it != target_config.end(); ++it )
    {

    }
    throw error( "project::apply_target_configuration::Not yet implemented." );
}

// Configuration is applied, order is fixed. Generate output names
/*const string_vector project::determine_output_files()
{
    for( auto it = m_targets.begin(); it != m_targets.end(); ++it )
    {

    }
}*/

void project::generate_commands()
{
    throw error( "generate_commands is not implemented yet." );
}

void project::execute_build_commands()
{
    throw error( "execute_build_commands is not implemented yet." );
}

libambrosia_namespace_end
