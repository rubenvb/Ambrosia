/**
  * commandline.cpp
  * Function implementations.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function includes
#include "commandline.h"

// Ambrosia includes
#include "help_and_version_output.h"
#include "output.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
/* "Ambrosia/Configuration/ambrosia_config.h" */
#include "Ambrosia/debug.h"
    using libambrosia::debug;
#include "Ambrosia/Error/commandline_error.h"
#include "Ambrosia/Error/soft_error.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/project.h"
#include "Ambrosia/Error/error.h"

// C-ish includes
#include <cassert>

// C++ includes
#include <algorithm>
#include <sstream>
    using std::istringstream;
#include <string>
    using std::string;

ambrosia_namespace_begin

void apply_commandline_options( const string_vector &arguments, lib::project &project,
                                lib::file_cache &files )
{
    // Debug output
    std::for_each( arguments.begin(), arguments.end(),
                   [](const string &option)
                   {
                       static int i= 1;
                       debug(debug::type::commandline) << "commandline::apply_commandline_options::argument: " << i++ << ": " << option << ".\n";
                   } );
    debug(debug::commandline) << "commandline::apply_commandline_options::Number of commandline arguments: " << arguments.size() << ".\n";

    // Execution ending arguments
    if( lib::contains(arguments, string("-v")) || lib::contains(arguments, "-version") )
        throw lib::soft_error( "Execution stopped after request for version output." );
    else if( lib::contains(arguments, "-h") || lib::contains(arguments, "-help") )
    {
        print_help_information();
        throw lib::soft_error( "Execution stopped after request for help output." );
    }

    // Options to be filled in, with default values, if any
    string current;
    bool m_first_dashless_argument = true;
    const auto end = arguments.end();
    for( auto it = arguments.begin(); it != end; ++it )
    {
        current = *it;
        string::size_type index = current.find_first_not_of( "-:" );
        switch( index )
        {
            case 0:
                // only allow the first dashless argument to serve as source directory or file path
                if( m_first_dashless_argument )
                {
                    m_first_dashless_argument = false;
                    debug(debug::commandline) << "begin::Possible project file or directory: \'" << current << "\'.\n";

                    files.find_project_file( current, lib::project::configuration );

                    // if project_file is still empty, "current" is really a target name to be built, skip to below next else
                    if( lib::project::configuration->project_file().empty() )
                        goto add_target;
                }
                else
                {
                    add_target:
                    if( !add_build_target(current) )
                        throw lib::commandline_error( "Unable to add target " + current + " to be built.",
                                                      std::distance(arguments.begin(),it) );
                }
                break;
            case 1:
                if( current[0] == '-' )
                {
                    const string::size_type index = current.find( "=",1 );
                    if( index == string::npos || index == current.size()-1 )
                        throw lib::commandline_error( "Ambrosia internal options must be set by \'-option=value\' type arguments.",
                                                      std::distance(arguments.begin(),it) );

                    const string option( current.substr(1,index-1) );
                    const string value( current.substr(index+1, string::npos) );
                    project.set_internal_option( option, value );
                }
                else if( current[0] == ':' )
                {
                    if( !add_configuration_options(current.substr(1), lib::project::configuration) )
                        throw lib::commandline_error( "Cannot set configuration option: " + current,
                                                      std::distance(arguments.begin(), it) );
                }
                break;
            case 2:
                // TODO: user arguments defined in project file:
                //  --<argument>=<value>
                //  store in a string_map?
                break;
            default:
                throw lib::commandline_error( "Invalid commandline argument: " + current,
                                              std::distance(arguments.begin(), it) );
        }
    }
    // if project file is not yet set, search current directory
    if( lib::project::configuration->project_file().empty() )
    {
        if( files.find_project_file(".", lib::project::configuration) )
        {
            debug(debug::commandline) << "begin::Project file found in current directory \'.\': "
                                      << lib::project::configuration->project_file() << ".\n";
            lib::emit_warning( "Ambrosia does not recommend an in-source build." );
        }
        else
            throw lib::error( "No project file found in specified path or current directory." );
    }
    debug(debug::commandline) << "commandline::apply_commandline_options::Checking if project file was found.\n";
    // Ensure that a valid project file has been found
    assert( lib::file_exists(lib::project::configuration->project_file()) );
}

bool add_build_target( const string &target )
{
    // TODO: fixme: this function does wrong things
    const string::size_type index = target.find( ":" );
    if( index == string::npos )
    {
        debug(debug::commandline) << "commandline::add_build_target::Target to be built: " << target << ".\n";
        lib::project::configuration->add_target_config_options( target, string_set() );
    }
    else
    {
        const string target_name( target.substr(0, index) );
        debug(debug::commandline) << "commandline::add_build_target::Target to be built: " << target_name << ".\n";
        string_set options;
        string_set duplicates;
        istringstream stream( target );
        stream.seekg( static_cast<istringstream::streamoff>(index) );
        string temp;
        while( std::getline(stream, temp, ',') )
        {
            if( options.insert(temp).second == false )
                duplicates.insert( temp );
        }
        lib::project::configuration->add_target_config_options( target_name, options );
    }
    return true;
}

void set_internal_option( const std::string &option, const std::string &value )
{
    //debug(3) << "begin::Ambrosia internal option: " << option
    //         << " with value " << value << " being set.\n";

    if( "cross" == option )
    {
        //debug(4) << "begin::Cross-compiling for " << value << ".\n";
        lib::s_ambrosia_config.set_ambrosia_cross( value );
    }
    #ifdef AMBROSIA_DEBUG
    else if( "d" == option || "debug" == option )
    {
        const uint32_t level = lib::from_string<uint32_t>( value );
        // check validity, partial check on input as well
        if( level > lib::s_max_debug_level )//|| level < 0 )
            lib::emit_error( "Debug level must be a number between 0 and 9." );
        //debug(0) << "begin::Setting debug level to " << level << ".\n";
        debug::s_level = static_cast<debug::type>(level);
    }
    #endif // AMBROSIA_DEBUG
    else if( "gnu-prefix" == option )
    {
        //debug(4) << "begin::Cross-compiling with GNU prefix " << value << ".\n";
        lib::project::configuration->set_gnu_prefix( value );
    }
    else
        lib::emit_error( "Unknown option passed to Ambrosia: \n\t-" + option + "=" + value );
}

bool add_configuration_options( const string &options, lib::ambrosia_config* /*config*/ )
{
    debug(debug::commandline) << "commandline::addd_configuration_options::Target configuration option: "
                              << options << " set.\n";
    // put them in a set
    istringstream stream( options );
    string temp;
    string_set new_options;
    while( getline(stream, temp, ',') )
    {
        if( new_options.insert(temp).second == false )
            output() << "Warning: duplicate configuration option: " << temp << ".\n";
    }

    // add config options to all previous targets and output duplicates as a warning.

    return true;

}

ambrosia_namespace_end
