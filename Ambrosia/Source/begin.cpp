/**
  * begin.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "begin.h"
#include "end_state.h"
#include "reader.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
    using libambrosia::debug;
#include "Ambrosia/enums.h"
#include "Ambrosia/status.h"
#include "help_and_version_output.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/nectar.h"

// Ambrosia includes
#include "output.h"
/* "Ambrosia/state.h" */
    using ambrosia::state;

// C++ includes
/* <string> */
    using std::string;
#include <sstream>
    using std::istringstream;
/* <vector> */
    using std::vector;

ambrosia_namespace_begin

begin::begin( const int argc, const char* const argv[], state* parent )
:   state( parent ),
    m_first_dashless_argument( true ),
    m_arguments( argv + 1, argv + argc )
{
    //debug(0) << "begin::Begin state created.\n";

    // Welcome message
    print_version_information();

    // other debug stuff
    for( int i=1; i<argc; ++i )
    {
        //debug(1) << "begin::argument: " << i << ": " << argv[i] << ".\n";
    }
    //debug(1) << "begin::Number of commandline arguments: " << m_arguments.size() << ".\n";
}
begin::~begin()
{
    //debug(0) << "begin::Destroyed.\n";
}

state* begin::event()
{
    //debug(1) << "begin::Processing commandline arguments.\n";

    // execution ending arguments
    if( lib::contains(m_arguments, string("-v")) || lib::contains(m_arguments, "-version") )
    {
        // version info already printed
        return new end_state( this );
    }
    else if( lib::contains(m_arguments, "-h") || lib::contains(m_arguments, "-help") )
    {
        print_help_information();
        return new end_state( this );
    }

    // Options to be filled in, with default values, if any
    string current;
    const auto end = m_arguments.end();
    for( auto it = m_arguments.begin(); it != end; ++it )
    {
        current = *it;
        string::size_type index = current.find_first_not_of( "-:" );
        switch( index )
        {
            case 0:
                // only allow the first bare argument to serve as source directory or file path
                if( m_first_dashless_argument )
                {
                    m_first_dashless_argument = false;
                    //debug(2) << "begin::Possible project file or directory: \'" << current << "\'.\n";

                    //find_project_file( current );
                    if( lib::error_status() )
                        return new end_state( this );

                    // if project_file is still empty, "current" is really a target name to be built, skip to below next else
                    if( lib::s_ambrosia_config.project_file().empty() )
                        goto add_target;
                }
                else
                {
                    add_target:
                    if( !add_build_target(current) )
                        return new end_state( this );
                }
                break;
            case 1:
                if( current[0] == '-' )
                {
                    if( "-h" == current || "-help" == current )
                    {
                        //debug(2) << "begin::Printing help information.\n";
                        ambrosia::print_help_information();
                        return new end_state( "All good", this );
                    }
                    else if( "-v" == current || "-version" == current)
                    {
                        //debug(2) << "begin::Printing version information.\n";
                        ambrosia::print_version_information();
                        return new end_state( "All good", this );
                    }
                    const string::size_type index = current.find( "=",1 );
                    if( index == string::npos || index == current.size()-1 )
                        return new end_state( "Ambrosia internal options must be set by \'-option=value\' type arguments." );

                    const string option( current.substr(1,index-1) );
                    const string value( current.substr(index+1, string::npos) );
                    set_internal_option( option, value );
                    // check for any error that may have happened in the above call to libAmbrosia
                    if( lib::error_status() )
                        return new end_state( this );
                }
                else if( current[0] == ':' )
                {
                    if( !add_configuration_options(current.substr(1)) )
                        return new end_state( this );
                }
                break;
            case 2:
                // TODO: user arguments defined in project file:
                //  --<argument>=<value>
                //  store in a string_map?
                break;
            default:
                return new end_state( "Invalid commandline argument: \'" + current + "\'.", this );
        }
    }
    // if project file is not yet set, search current directory
    if( lib::s_ambrosia_config.project_file().empty() )
    {
        /*const string project_file = lib::find_nectar_file( "." );
        if( !project_file.empty() )
        {
            //debug(2) << "begin::Project file found in current directory \'.\': " << project_file << ".\n";
            lib::emit_warning( "Ambrosia does not recommend an in-source build." );
            lib::s_ambrosia_config.set_source_directory( "");
            lib::s_ambrosia_config.set_project_file( project_file );
        }
        else if( !lib::error_status() )
            lib::emit_error( "No project file found in specified path or current directory." );*/
    }
    //debug(1) << "begin::Checking if project file was found.\n";
    // Ensure that a valid project file has been found
    if( lib::file_exists(lib::s_ambrosia_config.path_to_project_file()) )
        return new reader( this );
    else
        return new end_state( "No project file was found. Please specify a project file or a directory containing a single project file.", this );
}



bool begin::add_build_target( const std::string &target )
{
    // TODO: fixme: this function does wrong things
    const string::size_type index = target.find( ":" );
    if( index == string::npos )
    {
        //debug(3) << "begin::Target to be built: " << target << ".\n";
        lib::s_ambrosia_config.add_target_config_options( target, string_set() );
    }
    else
    {
        const string target_name( target.substr(0, index) );
        ///debug(3) << "begin::Target to be built: " << target_name << ".\n";
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
        lib::s_ambrosia_config.add_target_config_options( target_name, options );
    }
    return true;
}
void begin::set_internal_option( const std::string &option, const std::string &value )
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
        lib::s_ambrosia_config.set_gnu_prefix( value );
    }
    else
        lib::emit_error( "Unknown option passed to Ambrosia: \n\t-" + option + "=" + value );
}
bool begin::add_configuration_options( const std::string &options )
{
    //debug(3) << "begin::Target configuration option: " << options << " set.\n";
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
