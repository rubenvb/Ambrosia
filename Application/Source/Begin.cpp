/**
  * Begin.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Begin.h"

// libAmbrosia includes
#include "Ambrosia/Debug.h"
#include "Ambrosia/Platform.h"
#include "FileStore.h"

// Ambrosia includes
#include "Output.h"

// libAmbrosia includes
#include "Ambrosia/Global.h" // for version information

// C++ includes
#include <string>
    using std::string;
#include <sstream>
    using std::istringstream;
/* <vector> */
    using std::vector;

namespace ambrosia
{
    Begin::Begin( const int argc, const char* const argv[], State* parent )
    :   State( parent ),
        m_first_dashless_argument( true ),
        m_arguments( vector<string>() )
    {
        // Welcome message
        Output() << "Welcome to Ambrosia, version "
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
            Debug() << "Begin::argument: " << i << ": " << argv[i] << ".\n";
        }
        Debug() << "Begin::Number of commandline arguments: " << m_arguments.size() << ".\n";
    }
    Begin::~Begin()
    {
        Debug() << "Begin::Destroyed.\n";
    }

    State* Begin::event()
    {
        Debug() << "Begin::Processing commandline arguments.\n";

        const auto end = m_arguments.end();
        auto it = m_arguments.begin();
        string current = *it;

        // Options to be filled in, with default values, if any
        string source_directory( "." );
        string project_file;
        vector<pair_string_string_set> targets_and_options;
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
                        Debug() << "Begin::possible project file or directory:" << current << ".\n";
                        project_file = find_project_file( current );
                        // if the directory contains a *.nectar.txt file, set source directory as well
                        if( !project_file.empty() )
                        {
                            Debug() << "Begin::Project file found: " <<  project_file << ".\n";
                            source_directory = current;
                        }
                        else
                        {
                            // no project file found, search current directory
                            Debug() << "Begin::No *.nectar.txt file found in " << current << ".\n";
                            //project_file = find_project_file( source_directory );
                            if( project_file.empty() )
                                return new End( "Error:No project file found in " + current+ " or current directory.", this );
                        }
                    }
                    else
                    {
                        Debug() << "Begin::Target to be built: " << current << ".\n";
                        index = current.find( ":" );
                        if( index == string::npos )
                            targets_and_options.push_back( std::make_pair(current, "") );
                        else
                        {
                            string options;
                            istringstream stream( current );
                            stream.seekg( index );
                            string temp;
                            while( std::getline(stream, temp, ',') )
                            {
                                options += " " + temp;
                            }
                            targets_and_options.push_back( std::make_pair(current.substr(0, index), options) );
                        }
                    }
                    break;
                case 1:
                    if( current[0] == '-' )
                    {
                        Debug() << "Begin::Ambrosia option: " << current << " set.\n";

                    }
                    else if( current[0] == ':' )
                    {
                        Debug() << "Begin::Target configuration option: " << current << " set.\n";
                        // add config options to all previous targets.
                    }
                    break;
                default:
                    return new End( "Unknown commandline argument: \'" + current + "\'.", this );
            }
        }

        return new End( "Begin does little for now.", this );
    }

    const string Begin::find_project_file( const std::string &path )
    {
        // check if the path is a file
        if( ambrosia::file_exists(path) )
            return path;

        // check if it's a directory, nd search it for a *.nectar.txt file
        if( ambrosia::directory_exists(path) )
        {
            string project_file( FileStore::find_nectar_file(path) );
            if( !project_file.empty() )
                return project_file;
        }
        // if no *.nectar.txt file is found, return an empty string
        return string();
    }
} // namespace ambrosia
