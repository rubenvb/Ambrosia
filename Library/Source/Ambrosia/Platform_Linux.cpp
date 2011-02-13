/**
  * Platform_Linux.cpp
  * Function implementations for GNU/Linux.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Platform.h"

// C++ includes
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;

// Platform includes
#include "dirent.h"
#include "sys/stat.h"

namespace ambrosia
{
/*
 * Constants
 ************/
    const std::string executableSuffix = "";
    const os buildOS = os::Linux;
/*
 * Functions
 ************/
    template<class OutIt>
    void scanDir( OutIt it, const string &relDir, const string &dirName )
    {
        DIR* dir;
        struct dirent* entry;
        struct stat attributes;

        if( (dir=opendir(relDir.c_str())) == 0 )
            throw runtime_error( "unable to open file: " + dirName );

        // store cwd to return to original directory when finished
        string cwd( currentWorkingDir() );

        chdir( relDir.c_str() );

        while( (entry=readdir(dir)) != 0 )
        {
            string name = entry->d_name;
            int result = stat( entry->d_name, &attributes );
            if( result == -1 )
                throw runtime_error( "lstat failed..." );

            if( S_ISDIR(attributes.st_mode) )
            {
                // skip ".", "..", and ".*" (hidden directory on *nix)
                if( strcmp(".",name.substr(0,1).c_str()) == 0 || strcmp("..",name.c_str()) == 0 )
                    continue;
                else
                    scanDir( it, name, name );
            }
            else
            {
                if( dirName.empty() )
                    it = make_pair( name, attributes.st_mtime );
                else
                    it = make_pair( dirName+"/"+name, attributes.st_mtime );
            }
        }
        chdir( cwd.c_str() );
        closedir( dir );
    }
} // namespace ambrosia
