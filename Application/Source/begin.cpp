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
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"
#include "help_and_version_output.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/nectar.h"

// Ambrosia includes
#include "output.h"

// C++ includes
/* <string> */
    using std::string;
#include <sstream>
    using std::istringstream;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    begin::begin( const int argc, const char* const argv[], state* parent )
    :   state( parent ),
        m_first_dashless_argument( true ),
        m_arguments()
    {
        // serialize arguments in a string members
        m_arguments.reserve( argc );
        for( int i=1; i<argc; ++i )
        {
            m_arguments.push_back(argv[i]);
            debug() << "begin::argument: " << i << ": " << argv[i] << ".\n";
        }
        debug() << "begin::Number of commandline arguments: " << m_arguments.size() << ".\n";
    }
    begin::~begin()
    {
        debug() << "begin::Destroyed.\n";
    }

    state* begin::event()
    {
        debug() << "begin::Processing commandline arguments.\n";

        const auto end = m_arguments.end();
        string current;

        // Options to be filled in, with default values, if any
        for( auto it = m_arguments.begin(); it != end; ++it )
        {
            current = *it;
            size_t index = current.find_first_not_of( "-:" );
            switch( index )
            {
                case 0:
                    // only allow the first bare argument to serve as source directory or file path
                    if( m_first_dashless_argument )
                    {
                        m_first_dashless_argument = false;
                        debug() << "begin::Possible project file or directory: \'" << current << "\'.\n";

                        find_project_file( current );

                        if( ambrosia::current_status() == status::error )
                            return new end_state( this );
                        else if( ambrosia::current_status() == status::warning )
                            ambrosia::print_warnings();    // warning emitted when in-source build is performed

                        // if project_file is still empty, "current" is really a target name to be built, skip to below next else
                        if( s_build_config.project_file().empty() )
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
                            debug() << "begin::Printing help information.\n";
                            ambrosia::print_help_information();
                            return new end_state( "All good", this );
                        }
                        else if( "-v" == current || "-version" == current)
                        {
                            debug() << "begin::Printing version information.\n";
                            ambrosia::print_version_information();
                            return new end_state( "All good", this );
                        }
                        const size_t index = current.find( "=",1 );
                        if( index == string::npos || index == current.size()-1 )
                            return new end_state( "Ambrosia internal options must be set by \'-option=value\' type arguments." );

                        const string option( current.substr(1,index-1) );
                        const string value( current.substr(index+1, string::npos) );
                        set_internal_option( option, value );
                        // check for any error that may have happened in the above call to libAmbrosia
                        if( ambrosia::current_status() == status::error )
                            return new end_state( this );
                    }
                    else if( current[0] == ':' )
                    {
                        if( !add_configuration_options(current) )
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
        if( s_build_config.project_file().empty() )
        {
            const string project_file = ambrosia::find_nectar_file( "." );
            if( !project_file.empty() )
            {
                debug() << "begin::Project file found in current directory \'.\': " << project_file << ".\n";
                ambrosia::emit_warning( "Ambrosia does not recommend an in-source build." );
                s_build_config.set_source_directory( "");
                s_build_config.set_project_file( project_file );
            }
            else if( ambrosia::current_status() != status::error )
                ambrosia::emit_error( "No project file found in specified path or current directory." );
        }
        debug() << "begin::Checking if project file was found.\n";
        // Ensure that a valid project file has been found
        if( file_exists(s_build_config.path_to_project_file()) )
            return new reader( this );
        else
            return new end_state( "No project file was found. Please specify a project file or a directory containing a single project file.", this );
    }

    bool begin::find_project_file( const std::string &path )
    {
        debug() << "begin::find_project_file called for " << path << ".\n";

        if( ambrosia::file_exists(path) )
        {
            debug() << "begin::find_project_file detected file.\n";
            // TODO: generalize the directory seperators list
            // seperate filename from (realtive) path
            const size_t index = path.find_last_of( "/\\" );
            s_build_config.set_project_file( path.substr(index+1, string::npos) );
            s_build_config.set_source_directory( path.substr(0, index) );
            return true;
        }
        else if( ambrosia::directory_exists(path) )
        {
            const string project_file = ambrosia::find_nectar_file( path );
            // if the directory contains a *.nectar.txt file, set source directory as well
            if( !project_file.empty() )
            {
                debug() << "begin::Project file found: " <<  path << ambrosia::directory_seperator << project_file << ".\n";
                s_build_config.set_source_directory( path );
                s_build_config.set_project_file( project_file );
                return true;
            }
        }
        // return failure if some condition failed
        debug() << "begin::No *.nectar.txt file found in " << path << ".\n";
        return false;
    }

    bool begin::add_build_target( const std::string &target )
    {
        // TODO: fixme: this function does wrong things
        const size_t index = target.find( ":" );
        if( index == string::npos )
        {
            debug() << "begin::Target to be built: " << target << ".\n";
            s_build_config.add_target_config( target, string_set() );
        }
        else
        {
            const string target_name( target.substr(0, index) );
            debug() << "begin::Target to be built: " << target_name << ".\n";
            string_set options;
            string_set duplicates;
            istringstream stream( target );
            stream.seekg( index );
            string temp;
            while( std::getline(stream, temp, ',') )
            {
                if( options.insert(temp).second == false )
                    duplicates.insert( temp );
            }
            s_build_config.add_target_config( target_name, options );
        }
        return true;
    }
    void begin::set_internal_option( const std::string &option, const std::string &value )
    {
        debug() << "begin::Ambrosia internal option: " << option
                << " with value " << value << " being set.\n";

        if( "cross" == option )
        {
            debug() << "begin::Cross-compiling for " << value << ".\n";
            s_build_config.set_ambrosia_cross( value );
        }
        else if( "gnu-prefix" == option )
        {
            debug() << "begin::Cross-compiling with GNU prefix " << value << ".\n";
            s_build_config.set_gnu_prefix( value );
        }
    }
    bool begin::add_configuration_options( const std::string &options )
    {
        debug() << "begin::Target configuration option: " << options << " set.\n";
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
} // namespace ambrosia
