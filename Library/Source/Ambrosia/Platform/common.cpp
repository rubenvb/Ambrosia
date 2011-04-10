/**
  * Ambrosia/Platform/common.cpp
  * Function implementations common accross OS'es.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "platform.h"

// Common platform includes
#if __linux__
    #include <sys/io.h>   // For access()
#else
    #include <io.h>
#endif
#include <sys/types.h>  // For stat()
#include <sys/stat.h>   // For stat()

// C++ includes
#include <memory>
    using std::unique_ptr;
#include <stdexcept>
    using std::runtime_error;

namespace ambrosia
{
/*
 * Common platform implementations
 **********************************/
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
    bool directory_exists( const std::string &directory )
    {
        if( !directory.empty() )
        {
            if( access(directory.c_str(), 0) == 0 )
            {
                struct stat status;
                stat( directory.c_str(), &status );
                if( status.st_mode & S_IFDIR )
                    return true;
            }
        }
        // if any condition fails
        return false;
    }
    bool file_exists( const std::string &filename )
    {
        if( !filename.empty() )
        {
            if( access(filename.c_str(), 0) == 0 )
            {
               struct stat status;
               stat( filename.c_str(), &status );
               if( !(status.st_mode & S_IFDIR) )
                   return true;
            }
        }
        // if any condition fails
        return false;
    }
} // namespace ambrosia
