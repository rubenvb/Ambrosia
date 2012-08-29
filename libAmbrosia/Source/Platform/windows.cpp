/**
 *
 * Project Ambrosia: Ambrosia library
 *
 * Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related
 * and neighboring rights to this software to the public domain worldwide. This software is
 * distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Ambrosia/Platform/windows.cpp
 * Function implementations for Windows.
 *
 **/

// Function include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
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
#include <direct.h>
#include <windows.h>
#include <shlwapi.h>

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

bool create_directory(const string& name)
{
  return CreateDirectoryW((L"\\\\?\\" + convert_to_utf16(name)).c_str(), NULL);
}
void create_directory_recursive(const string& name)
{
  //TODO: optimize the calls to convert_to_utf16 to only once: make and call create_directory_recursive(const wstring&)
  if(!CreateDirectoryW((/*L"\\\\?\\" + */convert_to_utf16(name)).c_str(), NULL))
  {
    DWORD win32_error = GetLastError();
    if(win32_error == ERROR_FILE_NOT_FOUND || win32_error == ERROR_PATH_NOT_FOUND)
    {
      string_pair split_name = split_preceding_directory(name);
      debug(debug::platform) << "platform::create_directory_recursive::Parent directory doesn't exist, creating \"" << split_name.first << "\".\n";
      create_directory_recursive(split_name.first);
    }
    else if(win32_error == ERROR_ALREADY_EXISTS)
    {
      debug(debug::platform) << "platform::create_directory_recursive::Directory " << name << " already exists.\n";
    }
    else
      throw error("Win32 error: CreateDirectoryW call failed for " + name + " with error: " + to_string(win32_error) + ".");
  }
}

int execute_command(const string &command,
                    string &string_cout,
                    string &string_cerr)
{
  // adapted from http://msdn.microsoft.com/en-us/library/ms682499%28VS.110%29.aspx
  SECURITY_ATTRIBUTES attributes;

  attributes.nLength = sizeof(SECURITY_ATTRIBUTES); // Set the bInheritHandle flag so pipe handles are inherited.
  attributes.bInheritHandle = TRUE;
  attributes.lpSecurityDescriptor = NULL;

  // Create a pipe for the child process's STDOUT
  HANDLE stdout_read_handle;
  HANDLE stdout_write_handle;
  if(!CreatePipe(&stdout_read_handle, &stdout_write_handle, &attributes, 0))
    throw error("Win32 error: Unable to create pipe for stdout.");
  // Create pipe for the child process's STDERR
  HANDLE stderr_read_handle;
  HANDLE stderr_write_handle;
  if(!CreatePipe(&stderr_read_handle, &stderr_write_handle, &attributes, 0))
    throw error("Win32 error: Unable to create pipe for stderr.");
  // Ensure the read handle to the pipe for STDOUT is not inherited.
  if(!SetHandleInformation(stdout_read_handle, HANDLE_FLAG_INHERIT, 0))
    throw error("Win32 error: Unable to set stdout read handle inheritability.");

  PROCESS_INFORMATION process_info = {0,0,0,0};
  STARTUPINFOW startup_info = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  startup_info.cb = sizeof(STARTUPINFOW);
  startup_info.hStdError = stderr_write_handle;
  startup_info.hStdOutput =stdout_write_handle;
  startup_info.dwFlags |= STARTF_USESTDHANDLES;

  std::wstring commandW = convert_to_utf16(command);

  if(!CreateProcessW(NULL, // app
                     &commandW[0], // command line
                     NULL, // process security attributes
                     NULL, // primary thread security attributes
                     TRUE, // handles are inherited
                     0, // creation flags
                     NULL, // use parent's environment
                     NULL, // use parent's current directory
                     &startup_info, // STARTUPINFOW pointer
                     &process_info)) // receives PROCESS_INFORMATION
    throw error("Win32 error: failed to call CreateProcess for command \'" + command + "\'\n"
                "\t with error: " + to_string(GetLastError()));

  debug(debug::platform) << "platform::execute_command::CreateProcess call successful.\n";

  // Close thread handle
  CloseHandle(process_info.hThread);
  // Get process exit code
  DWORD exit_code = STILL_ACTIVE;
  while(exit_code == STILL_ACTIVE)
  {
    if(!GetExitCodeProcess(process_info.hProcess, &exit_code))
      throw error("Win32 error: failed to call GetExitCodeProcess with error: " + to_string(GetLastError()));
  }
  debug(debug::platform) << "platform::execute_command::Process exit code: " << exit_code << "\n";

  // close process handle
  CloseHandle(process_info.hProcess);

  debug(debug::platform) << "platform::execute_command::Closed process_info handles.\n";

  // close write ends of pipes
  CloseHandle(stdout_write_handle);
  CloseHandle(stderr_write_handle);

  // Read from pipes
  constexpr size_t buffer_size = 1024;
  string buffer;
  buffer.resize(buffer_size);
  DWORD bytes_read = 0;

  debug(debug::platform) << "platform::execute_command::reading from stdout pipe.\n";
  while(ReadFile(stdout_read_handle, &buffer[0], buffer_size, &bytes_read, NULL) && bytes_read != 0)
  {
    string_cout.append(buffer.substr(0, static_cast<size_t>(bytes_read)+1));
    debug(debug::platform) << "platform::execute_command::Read " << bytes_read << " bytes from stdout pipe:\n"
                           << string_cout << "\n";
  }
  debug(debug::platform) << "platform::execute_command::reading from stderr pipe.\n";
  while(ReadFile(stderr_read_handle, &buffer[0], buffer_size, &bytes_read, NULL) && bytes_read != 0)
  {
    string_cerr.append(buffer.substr(0, static_cast<size_t>(bytes_read)+1));
    debug(debug::platform) << "platform::execute_command::Read " << bytes_read << " bytes from stderr pipe:\n"
                           << string_cerr << (string_cerr.empty() ? "" : "\n");
  }
  // Close pipe read handles
  CloseHandle(stdout_read_handle);
  CloseHandle(stderr_read_handle);

  return static_cast<int>(exit_code);
}

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
