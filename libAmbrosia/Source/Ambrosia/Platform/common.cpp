/**
  * Ambrosia/Platform/common.cpp
  * Function implementations common accross OS'es.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/debug.h"

// Common platform includes
#if _WIN32
    #define S_IFREG _S_IFREG
#endif // _WIN32
#if __linux__
    #include <sys/io.h>   // For access()
#else
    #include <io.h>
    #include <stdio.h>
#endif
#include <sys/types.h>  // For stat
#include <sys/stat.h>   // For (_w)stat()

// C++ includes
#include <memory>
    using std::unique_ptr;
#include <stdexcept>
    using std::runtime_error;
#include <string>
    using std::string;
    using std::wstring;
/* <vector> */
    using std::vector;

/*
 * Workarounds
 **************/
#if _WIN32
# define getcwd _getcwd
#endif // _WIN32

libambrosia_namespace_begin

/*
 * Common platform implementations
 **********************************/
const vector<string> get_environment_PATH()
{
#if _WIN32
    const std::string PATH = convert_to_utf8( _wgetenv(L"PATH") );
    const char delimiter = ';';
#else
    const std::string PATH = getenv( "PATH" );
    const char delimiter = ':';
#endif
    if( PATH.empty() )
        throw runtime_error( "PATH should not be empty" );

    vector<string> result;
    size_t previous = 0;
    size_t index = PATH.find( delimiter );
    while( index != string::npos )
    {
        result.push_back( PATH.substr(previous, index-previous));
        debug(debug::type::platform) << "platform::get_environment_PATH::part of PATH: " << result.back() << "\n";
        previous=index+1;
        index = PATH.find( delimiter, previous );
    }
    result.push_back( PATH.substr(previous) );

    return result;
}

const std::string current_working_directory()
{
    const int chunkSize=255;
    const int maxChunks=10240; // 2550 KiBs of current path are more than enough

    char stackBuffer[chunkSize]; // Stack buffer for the "normal" case
    if( getcwd(stackBuffer, sizeof(stackBuffer)) != NULL )
        return stackBuffer;
    if( errno!=ERANGE )
      throw std::runtime_error("Cannot determine the current path.");

    for(int chunks=2; chunks<maxChunks ; chunks++)
    {
        std::unique_ptr<char> cwd(new char[chunkSize*chunks]);
        if( getcwd(cwd.get(),chunkSize*chunks) != 0 )
            return cwd.get();

        if(errno!=ERANGE)
            throw std::runtime_error("Cannot determine the current path.");
    }
    throw std::runtime_error("Cannot determine the current path; the path is apparently unreasonably long");
}
bool directory_exists( const string &directory )
{
#if _WIN32
    wstring directoryW( convert_to_utf16(directory) );
    struct _stat64 status;
    if( _wstat64(directoryW.c_str(), &status) == 0 )
#else // _WIN32
    struct stat status;
    if( stat(directory.c_str(), &status) == 0 )
#endif //_WIN32
    {
        debug(debug::type::platform) << "platform::directory_exists::(_w)stat(64) succeeded for " << directory << ".\n";
        if ( status.st_mode & S_IFDIR )
            return true;
    }
    debug(debug::type::platform) << "platform::directory_exists::" << directory << " is not a directory.\n";
    return false;
}
bool file_exists( const string &filename )
{
#if _WIN32
    wstring filenameW( convert_to_utf16(filename) );
    struct _stat64 status;
    if( _wstat64(filenameW.c_str(), &status) == 0 )
#else // _WIN32
    struct stat status;
    if( stat(filename.c_str(), &status) == 0 )
#endif //_WIN32
    {
        debug(debug::type::platform) << "platform::file_exists::(_w)stat(64) succeeded.\n";
        if ( status.st_mode & S_IFREG )
            return true;
    }
    debug(debug::type::platform) << "platform::file_exists::" << filename << " is not a file.\n";
    return false;
}

libambrosia_namespace_end
