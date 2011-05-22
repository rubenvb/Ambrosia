/**
  * Ambrosia/Platform/linux.cpp
  * Function implementations for GNU/Linux.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "platform.h"

// libAmbrosia includes
#include "typedefs.h"

// Platform includes
#include "dirent.h"
#include "sys/stat.h"

// C++ includes
#include <iterator>
    using std::back_insert_iterator;
    using std::insert_iterator;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
/* <vector> */
    using std::vector;

// C-ish includes
#include <cstring>

libambrosia_namespace_begin

/*
 * Constants
 ************/
const char directory_seperator = '/';
const std::string executableSuffix = "";
const os build_os = os::Linux;
// this works for GCC:
#if i386
    const architecture build_architecture = architecture::x86;
#elif __x86_64__
    const architecture build_architecture = architecture::amd64;
#endif // i386 || __x86_64__

/*
 * Functions
 ************/
template<class output_iterator>
void scan_directory( output_iterator it, const string &directory_name )
{
    DIR* dir;
    struct dirent* entry;
    struct stat attributes;

    if( (dir=opendir(directory_name.c_str())) == 0 )
        throw runtime_error( "unable to open file: " + directory_name );

    // store cwd to return to original directory when finished
    string cwd( current_working_directory() );

    chdir( directory_name.c_str() );

    while( (entry=readdir(dir)) != 0 )
    {
        const string name = entry->d_name;
        int result = stat( entry->d_name, &attributes );
        if( result == -1 )
            throw runtime_error( "lstat failed..." );

        if( S_ISREG(attributes.st_mode) )
            it = { name, attributes.st_mtime };
    }
    chdir( cwd.c_str() );
    closedir( dir );
}
template void scan_directory<insert_iterator<file_set> >( insert_iterator<file_set>, const string & );

template<class output_iterator>
void recursive_scan_directory( output_iterator it, const string &relative_directory, const string &directory_name )
{
    DIR* dir;
    struct dirent* entry;
    struct stat attributes;

    if( (dir=opendir(relative_directory.c_str())) == 0 )
        throw runtime_error( "unable to open file: " + directory_name );

    // store cwd to return to original directory when finished
    string cwd( current_working_directory() );

    chdir( relative_directory.c_str() );

    while( (entry=readdir(dir)) != 0 )
    {
        const string name = entry->d_name;
        int result = stat( entry->d_name, &attributes );
        if( result == -1 )
            throw runtime_error( "lstat failed..." );

        if( S_ISDIR(attributes.st_mode) )
        {
            // skip ".", "..", and ".*" (hidden directory on *nix)
            if( strcmp(".",name.substr(0,1).c_str()) == 0
             || strcmp("..",name.c_str()) == 0 )
                continue;
            else
                recursive_scan_directory( it, name, name );
        }
        else
        {
            if( directory_name.empty() )
                it = { name, attributes.st_mtime };
            else
                it = { directory_name + "/" + name, attributes.st_mtime };
        }
    }
    chdir( cwd.c_str() );
    closedir( dir );
}
// explicit instantiation
template void recursive_scan_directory<insert_iterator<file_set>>( insert_iterator<file_set>, const string &, const string & );

libambrosia_namespace_end
