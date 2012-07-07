/**
  * Ambrosia/Platform/windows.cpp
  * Function implementations for Windows.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <algorithm>
#include <iostream>
  using std::istream;
  using std::ostream;
#include <fstream>
  using std::ifstream;
  using std::ofstream;
#include <iterator>
  using std::back_insert_iterator;
  using std::insert_iterator;
#include <map>
  using std::map;
#include <memory>
  using std::unique_ptr;
#include <stdexcept>
  using std::logic_error;
  using std::runtime_error;
#include <string>
  using std::string;
  using std::wstring;
#include <vector>
  using std::vector;

// Windows includes
#include "direct.h"
#include "windows.h"
#include "shlwapi.h"

// Includes for the last hackish section
#ifdef __GLIBCXX__
#include <ext/stdio_filebuf.h>
#endif // __GLIBCXX__

libambrosia_namespace_begin

// architecture detection - directly from msdn
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
BOOL is_wow64()
{
  BOOL bIsWow64 = FALSE;
  LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

  if(NULL != fnIsWow64Process)
  {
    if(!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
      throw runtime_error("Unable to execute fnIsWow64Process.");
  }
  return bIsWow64;
}

/*
 * Constants
 *********************/
const char directory_seperator = '\\';
const string executable_suffix = ".exe";
const os build_os = os::Windows;
#if _WIN64
  const architecture build_architecture = architecture::amd64;
  const architecture ambrosia_architecture = architecture::amd64;
#else
  const architecture build_architecture = is_wow64() ? architecture::x86 : architecture::amd64;
  const architecture ambrosia_architecture = architecture::x86;
#endif // _WIN64

/*
 * Windows support functions
 ****************************/
// UTF16 -> UTF8 conversion
const string convert_to_utf8(const wstring& utf16_string)
{
  // get length
  int length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                                   NULL, 0, NULL, NULL);
  if(!(length > 0))
    return string();
  else
  {
    string result;
    result.resize(static_cast<string::size_type>(length));

    if(WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                           &result[0], static_cast<int>(result.size()), NULL, NULL) == 0 )
      throw runtime_error("Failure to execute toUTF8: conversion failed.");
    else
      return result;
  }
}
// UTF8 -> UTF16 conversion
const wstring convert_to_utf16(const string& utf8_string)
{
  // get length
  int length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), static_cast<int>(utf8_string.size()), NULL, 0);
  if(!(length > 0))
    return wstring();
  else
  {
    wstring result;
    result.resize(static_cast<string::size_type>(length));

    if(MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), static_cast<int>(utf8_string.size()),
                           &result[0], static_cast<int>(result.size())) == 0 )
      throw runtime_error("Failure to execute toUTF16: conversion failed.");
    else
      return result;
  }
}
// FILETIME to time_t conversion
time_t get_time(FILETIME const& ft)
{
  ULARGE_INTEGER ull;
  ull.LowPart = ft.dwLowDateTime;
  ull.HighPart = ft.dwHighDateTime;
  return static_cast<time_t>(ull.QuadPart / 10000000ULL - 11644473600ULL);
}


/*
 * Mostly platform dependently implemented functions
 ****************************************************/
bool is_absolute_path(const string& path)
{
  return !PathIsRelativeW(convert_to_utf16(path).c_str());
}

template<class output_iterator>
void scan_directory(output_iterator it,
                    const string& directory_name)
{
  const wstring directory_name_wide = convert_to_utf16( directory_name );
  wstring directory_wide( directory_name_wide + L"\\*" );

  _WIN32_FIND_DATAW find_data;

  HANDLE handle = FindFirstFileW(directory_wide.c_str(), &find_data);
  if(handle == INVALID_HANDLE_VALUE)
    return;

  // check each file/directory in structure
  while(true)
  {
    if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      it = {convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime)};
    if(!FindNextFileW(handle, &find_data) && GetLastError() == ERROR_NO_MORE_FILES)
      break;
  }
}
template void scan_directory<insert_iterator<file_set>>(insert_iterator<file_set>, const string&);

template<class output_iterator>
void recursive_scan_directory(output_iterator it,
                              const string& relative_directory,
                              const string& directory_name)
{
  const wstring directory_name_wide = convert_to_utf16(directory_name);
  const wstring relative_directory_wide = convert_to_utf16(relative_directory);
  wstring directory_wide;

  if(relative_directory.empty())
    directory_wide = directory_name_wide + L"\\*";
  else
    directory_wide = relative_directory_wide + L"/" + directory_name_wide + L"\\*";

  _WIN32_FIND_DATAW find_data;

  HANDLE handle = FindFirstFileW( directory_wide.c_str(), &find_data );
  if(handle == INVALID_HANDLE_VALUE)
    return;

  // check each file/directory in structure
  while(true)
  {
    if(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if((wcsncmp(L".", find_data.cFileName, 1) != 0) && (wcscmp(L"..", find_data.cFileName) != 0))
      {
        string subdirectory;
        if(directory_name.empty())
          subdirectory = convert_to_utf8(find_data.cFileName);
        else
          subdirectory = directory_name + "/" + convert_to_utf8( find_data.cFileName );

        recursive_scan_directory( it, relative_directory, subdirectory );
      }
    }
    else
    {
      if(directory_name.empty())
        it = {convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime)};
      else
        it = {directory_name + "/" + convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime)};

    }
    if(!FindNextFileW(handle, &find_data) && GetLastError() == ERROR_NO_MORE_FILES)
      break;
  }
}
// explicit instantiation
template void recursive_scan_directory<insert_iterator<file_set>>(insert_iterator<file_set>, const string&, const string&);

/*
 * Ugly workarounds
 *******************/
#if _WIN32
# if __GLIBCXX__
unique_ptr<istream> open_ifstream(const string& filename)
{
  FILE* c_file = _wfopen(convert_to_utf16(filename).c_str(), L"r");
  __gnu_cxx::stdio_filebuf<char>* buffer = new __gnu_cxx::stdio_filebuf<char>(c_file, std::ios_base::in, 1);

  return std::unique_ptr<istream>(new istream(buffer));
}
unique_ptr<ostream> open_ofstream(const string& filename)
{
  FILE* c_file = _wfopen(convert_to_utf16(filename).c_str(), L"w+");
  __gnu_cxx::stdio_filebuf<char>* buffer = new __gnu_cxx::stdio_filebuf<char>(c_file, std::ios_base::out, 1);

  return unique_ptr<ostream>(new ostream(buffer));
}
# elif _MSC_VER
unique_ptr<ifstream> open_ifstream(const string& filename)
{
  return unique_ptr<ifstream>(new ifstream(convert_to_utf16(filename)));
}
unique_ptr<ofstream> open_ofstream(const string& filename)
{
  return unique_ptr<ofstream>(new ofstream(convert_to_utf16(filename)));
}

# else
// Warning! unknown fstream implementation - no unicode filename support
unique_ptr<ifstream> open_ifstream(const string& filename)
{
  return unique_ptr<ifstream>(new ifstream(filename));
}
unique_ptr<ofstream> open_ofstream(const string& filename)
{
  return unique_ptr<ofstream>(new ofstream(filename));
}
# endif
#endif

libambrosia_namespace_end
