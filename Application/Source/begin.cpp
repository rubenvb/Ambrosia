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
#include "Ambrosia/error.h"
#include "help_and_version_output.h"
#include "Ambrosia/platform.h"
#include "file_store.h"

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
        auto it = m_arguments.begin();
        string current = *it;

        // Options to be filled in, with default values, if any
        string project_file;
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
                        debug() << "begin::possible project file or directory:" << current << ".\n";
                        project_file = find_project_file( current );
                        // if the directory contains a *.nectar.txt file, set source directory as well
                        if( !project_file.empty() )
                        {
                            debug() << "begin::Project file found: " <<  project_file << ".\n";
                            s_build_config.set_source_directory( current );
                        }
                        else
                        {
                            if( error::status::error == error::current_status() )
                                return new end_state( this );
                            // no project file found, search current directory
                            debug() << "begin::No *.nectar.txt file found in " << current << ".\n";
                            project_file = find_project_file( s_build_config.source_directory() );
                            if( project_file.empty() )
                            {
                                if( error::status::error == error::current_status() )
                                    return new end_state( this );
                                else
                                    return new end_state( "Error:No project file found in " + current + " or current directory.", this );
                            }
                            else
                            {
                                debug() << "begin::Project file found in current directory \'.\': " << project_file << ".\n";
                                // "current" is really a target name to be built, skip to below next else
                                goto add_target;
                            }
                        }
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
                        if( error::status::error == error::current_status()  )
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
        return new reader( this );
    }

    const string begin::find_project_file( const std::string &path )
    {
        debug() << "begin::find_project_file called for path = " << path << ".\n";
        // check if the path is a file
        if( ambrosia::file_exists(path) )
        {
            debug() << "begin::File specified on commandline.\n";
            return path;
        }
        // check if it's a directory, and search it for a *.nectar.txt file
        else if( ambrosia::directory_exists(path) )
        {
            debug() << "begin::Directory specified on commandline.\n";
            string project_file( file_store::find_nectar_file(path) );
            if( !project_file.empty() )
            {
                debug() << "begin::Project file found in specified directory.\n";
                return project_file;
            }
        }
        debug() << "begin::Project file not found in specified directory or more than one file located.\n";
        // if no *.nectar.txt file is found, return an empty string
        return string();
    }

    bool begin::add_build_target( const std::string &target )
    {
        // TODO: fixme: this function does wrong things
        const size_t index = target.find( ":" );
        if( index == string::npos )
        {
            debug() << "begin::Target to be built: " << target << ".\n";
            //m_targets_and_options.push_back( std::make_pair(target, string_set()) );
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
            //m_targets_and_options.push_back( std::make_pair(target_name, options) );
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
