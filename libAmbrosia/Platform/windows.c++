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
 * Platform/windows.c++
 * Function implementations for Windows.
 *
 **/

// Function include
#include "Ambrosia/platform.h++"

// libAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/Error/error.h++"
#include "Ambrosia/typedefs.h++"

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
#include <utility>
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

namespace ambrosia
{
namespace lib
{
namespace platform
{

// architecture detection - directly from msdn
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
BOOL is_wow64()
{
  BOOL bIsWow64 = FALSE;
  LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

  if(nullptr != fnIsWow64Process)
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

void command::set_program(const std::string& program_path)
{
  arguments = convert_to_utf16(program_path);
}
void command::add_argument(const std::string& argument)
{
  // append a space if not the first argument
  if(!argument.empty())
  {
    if(!arguments.empty())
      arguments.append(L" ");

    arguments.append(convert_to_utf16(argument));
  }
}
void command::add_arguments(const command& other_command)
{
  if(!other_command.arguments.empty())
  {
    if(!arguments.empty())
      arguments.append(L" ");

    arguments.append(other_command.arguments);
  }
}

/*
 * Windows support functions
 ****************************/
// UTF16 -> UTF8 conversion
const string convert_to_utf8(const wstring& utf16_string)
{
  // get length
  int length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                                   nullptr, 0, nullptr, nullptr);
  if(!(length > 0))
    return string();
  else
  {
    string result;
    result.resize(static_cast<string::size_type>(length));

    if(WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), static_cast<int>(utf16_string.size()),
                           &result[0], static_cast<int>(result.size()), nullptr, nullptr) == 0 )
      throw runtime_error("Failure to execute toUTF8: conversion failed.");
    else
      return result;
  }
}
// UTF8 -> UTF16 conversion
const wstring convert_to_utf16(const string& utf8_string)
{
  // get length
  int length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), static_cast<int>(utf8_string.size()), nullptr, 0);
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

void read_from_pipe(const HANDLE read_end, string& result)
{
  DWORD bytes_available = 0;
  while(PeekNamedPipe(read_end, nullptr, 0, nullptr, &bytes_available, nullptr) && bytes_available > 0)
  {
    debug(debug::platform, "windows::execute_command::", bytes_available, " bytes available on the stdout pipe.\n");
    string buffer;
    buffer.resize(bytes_available);
    DWORD bytes_read = 0;
    if(ReadFile(read_end, &buffer[0], bytes_available, &bytes_read, nullptr) && bytes_read != 0)
    {
      result.append(buffer.substr(0, static_cast<size_t>(bytes_read)+1));
      debug(debug::platform, "windows::execute_command::Read ", bytes_read, " bytes from stdout pipe:\n",
                             buffer.substr(0, static_cast<size_t>(bytes_read)+1), "\n");
    }
  }
}

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
string_vector get_commandline_arguments(char*[],
                                        const int)
{
  int argc;
  wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc); // check failure!
  string_vector result;
  result.reserve(static_cast<string_vector::size_type>(argc));
  for(int i=0; i<argc; ++i)
  {
    result.push_back(convert_to_utf8(argv[i]));
  }
  return result;
}

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
      it = file(convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime));
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

  HANDLE handle = FindFirstFileW(directory_wide.c_str(), &find_data);
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
        it = file(convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime));
      else
        it = file(directory_name + "/" + convert_to_utf8(find_data.cFileName), get_time(find_data.ftLastWriteTime));

    }
    if(!FindNextFileW(handle, &find_data) && GetLastError() == ERROR_NO_MORE_FILES)
      break;
  }
}
// explicit instantiation
template void recursive_scan_directory<insert_iterator<file_set>>(insert_iterator<file_set>, const string&, const string&);

/*bool create_directory(const string& name)
{
  // MSVC C4800 without the "0 !="
  return 0 != CreateDirectoryW((L"\\\\?\\" + convert_to_utf16(name)).c_str(), nullptr);
}
//DOESN'T WORK FOR RECURSIVE DIRECTORIES
void create_directory_recursive(const string& name)
{
  //TODO: optimize the calls to convert_to_utf16 to only once: make and call create_directory_recursive(const wstring&)
  if(!CreateDirectoryW((L"\\\\?\\" + convert_to_utf16(name)).c_str(), nullptr))
  {
    DWORD win32_error = GetLastError();
    if(win32_error == ERROR_FILE_NOT_FOUND || win32_error == ERROR_PATH_NOT_FOUND)
    {
      string_pair split_name = split_preceding_directory(name);
      debug(debug::platform) << "windows::create_directory_recursive::Parent directory doesn't exist, creating " << split_name.first << ".\n";
      create_directory_recursive(split_name.first);
    }
    else if(win32_error == ERROR_ALREADY_EXISTS)
    {
      debug(debug::platform) << "windows::create_directory_recursive::Directory " << name << " already exists.\n";
    }
    else
      throw error("Win32 error: CreateDirectoryW call failed for " + name + " with error: " + to_string(win32_error) + ".");
  }
}*/

std::pair<bool, int> execute_command(const platform::command& command,
                                     string &string_cout,
                                     string &string_cerr)
{
  // adapted from http://msdn.microsoft.com/en-us/library/ms682499(VS.110).aspx
  SECURITY_ATTRIBUTES attributes;

  attributes.nLength = static_cast<DWORD>(sizeof(SECURITY_ATTRIBUTES));
  attributes.bInheritHandle = TRUE; // Set the bInheritHandle flag so pipe handles are inherited.
  attributes.lpSecurityDescriptor = nullptr;

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

  PROCESS_INFORMATION process_info = {0,0,0,0};
  STARTUPINFOW startup_info = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  startup_info.cb = static_cast<int>(sizeof(STARTUPINFOW));
  startup_info.hStdError = stderr_write_handle;
  startup_info.hStdOutput =stdout_write_handle;
  startup_info.dwFlags |= STARTF_USESTDHANDLES;

  if(!CreateProcessW(nullptr, // app
                     &command.arguments[0], // commandline arguments
                     nullptr, // process security attributes
                     nullptr, // primary thread security attributes
                     TRUE, // handles are inherited
                     0, // creation flags
                     nullptr, // use parent's environment
                     nullptr, // use parent's current directory
                     &startup_info, // STARTUPINFOW pointer
                     &process_info)) // receives PROCESS_INFORMATION
    return std::make_pair(false, -1); // return false to signify process creation error

  debug(debug::platform, "windows::execute_command::CreateProcess call successful for ", command, "\n");

  // Close thread handle
  CloseHandle(process_info.hThread);
  // Get process exit code
  DWORD exit_code = STILL_ACTIVE;
  while(exit_code == STILL_ACTIVE)
  {
    if(!GetExitCodeProcess(process_info.hProcess, &exit_code))
      throw error("Win32 error: failed to call GetExitCodeProcess with error: " + to_string(GetLastError()));

    read_from_pipe(stdout_read_handle, string_cout);
    read_from_pipe(stderr_read_handle, string_cerr);

    Sleep(10);
  }
  debug(debug::platform, "windows::execute_command::Process exit code: ", exit_code, "\n");

  // close process handle
  CloseHandle(process_info.hProcess);

  debug(debug::platform, "windows::execute_command::Closed process_info handles.\n");

  // close write ends of pipes
  CloseHandle(stdout_write_handle);
  CloseHandle(stderr_write_handle);

  read_from_pipe(stdout_read_handle, string_cout);
  read_from_pipe(stderr_read_handle, string_cerr);
  // Close pipe read handles
  CloseHandle(stdout_read_handle);
  CloseHandle(stderr_read_handle);

  return std::make_pair(true, /*static_cast<int>(*/exit_code);
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

} // namespace platform

} // namespace lib

} // namespace ambrosia
