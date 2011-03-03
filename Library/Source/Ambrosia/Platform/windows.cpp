/**
  * ambrosia/platform/windows.cpp
  * Function implementations for Windows.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "platform.h"

// libAmbrosia includes
#include "debug.h"
#include "typedefs.h"

// C++ includes
#include <iterator>
    using std::back_insert_iterator;
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
#include <vector>
    using std::vector;

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
    const char directory_seperator = '\\';
    const string executable_suffix = ".exe";
    const os build_os = os::Windows;
#if _WIN64
    const architecture build_architecture = architecture::amd64;
#else
    const architecture build_architecture = architecture::x86;
#endif // _WIN64
/*
 * Windows support functions
 ****************************/
    // UTF16 -> UTF8 conversion
    const string convert_to_utf8( const wstring &utf16_string )
    {
        // get length
        int length = WideCharToMultiByte( CP_UTF8, 0,
                                          utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                                          NULL, 0,
                                          NULL, NULL );
        if( !(length > 0) )
            return string();
        else
        {
            string result;
            result.resize( length );

            if( WideCharToMultiByte(CP_UTF8, 0,
                                    utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                                    &result[0], static_cast<int>(result.size()),
                                    NULL, NULL)
                                               > 0 )
                return result;
            else
                throw runtime_error( "Failure to execute toUTF8: conversion failed." );
        }
    }
    // UTF8 -> UTF16 conversion
    const wstring convert_to_utf16( const string &utf8_string )
    {
        // get length
        int length = MultiByteToWideChar( CP_UTF8, 0,
                                          utf8_string.c_str(), static_cast<int>(utf8_string.size()),
                                          NULL, 0 );
        if( !(length > 0) )
            return wstring();
        else
        {
            wstring result;
            result.resize( length );

            if( MultiByteToWideChar(CP_UTF8, 0,
                                    utf8_string.c_str(), static_cast<int>(utf8_string.size()),
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
    bool directory_exists( const string &directory )
    {
        DWORD attributes = GetFileAttributesW( convert_to_utf16(directory).c_str() );
        if( attributes != INVALID_FILE_ATTRIBUTES
            && (attributes & FILE_ATTRIBUTE_DIRECTORY) )
            return true;
        else
            return false;
    }
    bool file_exists( const string &filename )
    {
        DWORD attributes = GetFileAttributesW( convert_to_utf16(filename).c_str() );
        // Win32 API magic
        if( 0xFFFFFFFF == attributes )
            return false;
        else
            return true;
    }

    template<class output_iterator>
    void scan_directory( output_iterator it, const std::string &directory_name )
    {
        const wstring directory_name_wide = convert_to_utf16( directory_name );
        wstring directory_wide( directory_name_wide + L"\\*" );
        debug() << "Platform::Scanning directory: " << convert_to_utf8( directory_wide ) << ".\n";

        _WIN32_FIND_DATAW find_data;

        HANDLE handle = FindFirstFileW( directory_wide.c_str(), &find_data );
        if( handle == INVALID_HANDLE_VALUE )
            return;//throw runtime_error( "scanDir: cannot access: " + convert_to_utf8(directory_wide) );

        // check each file/directory in structure
        while( true )
        {
            if( !(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                it = convert_to_utf8(find_data.cFileName);
            }
            if( !FindNextFileW(handle, &find_data) && GetLastError() == ERROR_NO_MORE_FILES )
                break;
        }
    }
    template void scan_directory<back_insert_iterator<vector<string> > >( back_insert_iterator<vector<string> >, const string & );

    template<class output_iterator>
    void recursive_scan_directory( output_iterator it, const string &relative_directory, const string &directory_name )
    {
        const wstring directory_name_wide = convert_to_utf16( directory_name );
        const wstring relative_directory_wide = convert_to_utf16( relative_directory );
        wstring directory_wide;

        if( relative_directory.empty() )
            directory_wide = directory_name_wide + L"\\*";
        else
            directory_wide = relative_directory_wide + L"\\" + directory_name_wide + L"\\*";

        _WIN32_FIND_DATAW find_data;

        HANDLE handle = FindFirstFileW( directory_wide.c_str(), &find_data );
        if( handle == INVALID_HANDLE_VALUE )
            return;//throw runtime_error( "scanDir: cannot access: " + toUTF8(dirW) );

        // check each file/directory in structure
        while( true )
        {
            if( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                if( (wcsncmp(L".", find_data.cFileName, 1) != 0) &&
                    (wcscmp(L"..", find_data.cFileName) != 0) )
                {
                    string subdirectory;
                    if( directory_name.empty() )
                        subdirectory = convert_to_utf8( find_data.cFileName );
                    else
                        subdirectory = directory_name + "/" + convert_to_utf8( find_data.cFileName );
                    recursive_scan_directory( it, relative_directory, subdirectory );
                }
            }
            else
            {
                if( directory_name.empty() )
                    it = make_pair( convert_to_utf8(find_data.cFileName), getTime(find_data.ftLastWriteTime) );
                else
                    it = make_pair( directory_name+"/"+convert_to_utf8(find_data.cFileName), getTime(find_data.ftLastWriteTime) );

            }
            if( !FindNextFileW(handle, &find_data) && GetLastError() == ERROR_NO_MORE_FILES )
                break;
        }
    }
    // explicit instantiation
    template void recursive_scan_directory<insert_iterator<file_set> >( insert_iterator<file_set>, const string &, const string & );
} // namespace ambrosia

