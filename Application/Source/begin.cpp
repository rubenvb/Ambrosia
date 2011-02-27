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

// libAmbrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/error.h"
#include "Ambrosia/platform.h"
#include "file_store.h"

// Ambrosia includes
#include "output.h"

// C++ includes
#include <algorithm>
#include <iterator>
    using std::insert_iterator;
#include <string>
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
        m_arguments( vector<string>() ),
        m_build_config( build_config() )
    {
        // Welcome message
        output() << "Welcome to Ambrosia, version "
                   << app_version_major << "."
                   << app_version_minor << "."
                   << app_version_bugfix << ", using libAmbrosia version "
                   << lib_version_major << "."
                   << lib_version_minor << "."
                   << lib_version_bugfix << ".\n";

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
        string source_directory( "." );
        string project_file;
        string user_options;
        string ambrosia_options;
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
                            source_directory = current;
                        }
                        else
                        {
                            // no project file found, search current directory
                            debug() << "begin::No *.nectar.txt file found in " << current << ".\n";
                            //project_file = find_project_file( source_directory );
                            if( project_file.empty() )
                                return new end_state( "Error:No project file found in " + current+ " or current directory.", this );
                        }
                    }
                    else
                    {
                        if( !add_build_target(current) )
                            return new end_state( this );
                    }
                    break;
                case 1:
                    if( current[0] == '-' )
                    {
                        if( !set_internal_option(current) )
                            return new end_state( this );
                    }
                    else if( current[0] == ':' )
                    {
                        if( !add_configuration_options(current) )
                            return new end_state( this );
                    }
                    break;
                default:
                    return new end_state( "Invalid commandline argument: \'" + current + "\'.", this );
            }
        }
        return new end_state( "begin does little for now.", this );
    }

    const string begin::find_project_file( const std::string &path )
    {
        // check if the path is a file
        if( ambrosia::file_exists(path) )
        {
            debug() << "begin::File specified on commandline.\n";
            return path;
        }

        // check if it's a directory, and search it for a *.nectar.txt file
        if( ambrosia::directory_exists(path) )
        {
            debug() << "begin::Directory specified on commandline.\n";
            string project_file( file_store::find_nectar_file(path) );
            if( !project_file.empty() )
            {
                debug() << "begin::Project file found in specified directory.\n";
                return project_file;
            }
        }
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
    bool begin::set_internal_option( const std::string &option )
    {
        debug() << "begin::Ambrosia internal option: " << option << " set.\n";
        error::emit_error( "begin::No Ambrosia internal options implemented yet." );
        return false;
    }
    bool begin::add_configuration_options( const std::string &options )
    {
        debug() << "begin::Target configuration option: " << options << " set.\n";
        // put them in a set
        istringstream stream( options );
        string temp;
        string_set new_options;
        while( stream >> temp )
        {
            if( new_options.insert(temp).second == false )
                output() << "Warning: duplicate configuration option: " << temp << ".\n";
        }
        // add config options to all previous targets and output duplicates as a warning.
        /*const auto end = m_build_config.end();
        for( auto it = m_targets_and_options.begin(); it != end; ++it )
        {
            string_set &old_options( (*it).second );
            string_set merged_options;
            string_set duplicate_options;
            // get merged set
            std::set_union( old_options.begin(), old_options.end(),
                            new_options.begin(), new_options.end(),
                            insert_iterator<string_set>(merged_options, merged_options.begin()) );
            // get duplicates
            std::set_intersection( old_options.begin(), old_options.end(),
                                   new_options.begin(), new_options.end(),
                                   insert_iterator<string_set>(duplicate_options, duplicate_options.begin()) );
            if( !duplicate_options.empty() )
            {
                Output() << "Warning: duplicate configuration options (for target " << (*it).first << "): ";
                const auto end = duplicate_options.end();
                for( auto it = duplicate_options.begin(); it != end; ++it )
                {
                    Output() << "Warning: duplicate configuration options: " << *it << "\n";
                }
            }
        }*/
        return true;
    }
} // namespace ambrosia
