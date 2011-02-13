/**
  * Platform.cpp
  * Function implementations for Windows.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Platform.h"

// C++ includes
#include <iterator>
    using std::insert_iterator;
#include <map>
    using std::map;
#include <memory>
    using std::unique_ptr;
#include <stdexcept>
    using std::runtime_error;
/* <string> */
    using std::string;
    using std::wstring;

// Windows includes
#include "direct.h"
#include "windows.h"

/*
 * Workarounds
 **************/
#define getcwd _getcwd

namespace ambrosia
{
/*
 * Constants
 *********************/
    const string executableSuffix = ".exe";
    const os buildOS = os::Windows;
/*
 * Windows support functions
 ****************************/
    // UTF16 -> UTF8 conversion
    const string toUTF8( const wstring &utf16string )
    {
        // get length
        int length = WideCharToMultiByte( CP_UTF8, 0,
                                          utf16string.c_str(), static_cast<int>(utf16string.size()),
                                          NULL, 0,
                                          NULL, NULL );
        if( !(length > 0) )
            return string();
        else
        {
            string result;
            result.resize( length );

            if( WideCharToMultiByte(CP_UTF8, 0,
                                    utf16string.c_str(), static_cast<int>(utf16string.size()),
                                    &result[0], static_cast<int>(result.size()),
                                    NULL, NULL)
                                               > 0 )
                return result;
            else
                throw runtime_error( "Failure to execute toUTF8: conversion failed." );
        }
    }
    // UTF8 -> UTF16 conversion
    const wstring toUTF16( const string &utf8string )
    {
        // get length
        int length = MultiByteToWideChar( CP_UTF8, 0,
                                          utf8string.c_str(), static_cast<int>(utf8string.size()),
                                          NULL, 0 );
        if( !(length > 0) )
            return wstring();
        else
        {
            wstring result;
            result.resize( length );

            if( MultiByteToWideChar(CP_UTF8, 0,
                                    utf8string.c_str(), static_cast<int>(utf8string.size()),
                                    &result[0], static_cast<int>(result.size()))
                                                              > 0 )
                return result;
            else
                throw runtime_error( "Failure to execute toUTF16: conversion failed." );
        }
    }
    // FILETIME to time_t conversion
    time_t getTime( const FILETIME &filetime )
    {
        time_t t2 = filetime.dwHighDateTime;
        t2 <<= 32;
        t2 |= filetime.dwLowDateTime;
        return t2;
    }

    const std::string currentWorkingDir()
    {
        const size_t chunkSize=255;
        const int maxChunks=10240; // 2550 KiBs of current path are more than enough

        char stackBuffer[chunkSize]; // Stack buffer for the "normal" case
        if( getcwd(stackBuffer, sizeof(stackBuffer)) != NULL )
            return stackBuffer;
        if( errno!=ERANGE )
          throw std::runtime_error("Cannot determine the current path.");

        for(int chunks=2; chunks<maxChunks ; chunks++)
        {
            std::unique_ptr<char> cwd(new char[chunkSize*chunks]);
            if( getcwd(cwd.get(),chunkSize*chunks) != NULL )
                return cwd.get();

            if(errno!=ERANGE)
                throw std::runtime_error("Cannot determine the current path.");
        }
        throw std::runtime_error("Cannot determine the current path; the path is apparently unreasonably long");
    }

/*
 * Functions
 ************/
    template<class OutIt>
    void scanDir( OutIt it, const string &relDir, const string &dirName )
    {
        const wstring dirNameW = toUTF16( dirName );
        const wstring relDirW = toUTF16( relDir );
        wstring dirW;

        if( relDir.empty() )
            dirW = dirNameW + L"\\*";
        else
            dirW = relDirW + L"\\" + dirNameW + L"\\*";

        _WIN32_FIND_DATAW findData;

        HANDLE handle = FindFirstFileW( dirW.c_str(), &findData );
        if( handle == INVALID_HANDLE_VALUE )
            return;//throw runtime_error( "scanDir: cannot access: " + toUTF8(dirW) );

        // check each file/directory in structure
        while( true )
        {
            if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                if( (wcsncmp(L".", findData.cFileName, 1) != 0) &&
                    (wcscmp(L"..", findData.cFileName) != 0) )
                {
                    string subDir;
                    if( dirName.empty() )
                        subDir = toUTF8( findData.cFileName );
                    else
                        subDir = dirName + "/" + toUTF8( findData.cFileName );
                    scanDir( it, relDir, subDir );
                }
            }
            else
            {
                if( dirName.empty() )
                    it = make_pair( toUTF8(findData.cFileName), getTime(findData.ftLastWriteTime) );
                else
                    it = make_pair( dirName+"/"+toUTF8(findData.cFileName), getTime(findData.ftLastWriteTime) );

            }
            if( !FindNextFileW(handle, &findData) && GetLastError() == ERROR_NO_MORE_FILES )
                break;
        }
    }
    // explicit instantiation
    template void scanDir<insert_iterator<map<string, time_t> > >( insert_iterator<map<string, time_t> >, const string &, const string & );
} // namespace ambrosia

