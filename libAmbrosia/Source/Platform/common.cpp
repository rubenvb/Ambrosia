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
 * Ambrosia/Platform/common.cpp
 * Function implementations common accross OS'es.
 *
 **/

// Function include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/debug.h"

// Platform includes
#if _WIN32
  #define S_IFREG _S_IFREG
  #include <direct.h>
  #include <stdio.h>
  #define getcwd _getcwd
  #define mkdir(path,mode) _wmkdir(path)
  #define snprintf _snwprintf
#endif
#if __linux__
  #include <sys/io.h> // For access()
  #include <unistd.h>
#else
  #include <io.h>
  #include <stdio.h>
#endif
#include <sys/types.h> // For stat
#include <sys/stat.h> // For (_w)stat()

// C++ includes
#include <cstddef>
  using std::size_t;
#include <cstring>
#ifdef _WIN32
  using std::wcslen;
#else
  using std::strlen;
#endif
#include <memory>
  using std::unique_ptr;
#include <stdexcept>
  using std::runtime_error;
#include <string>
  using std::string;
  using std::wstring;
#include <utility>
  using std::pair;
#include <vector>
  using std::vector;

namespace ambrosia
{
namespace lib
{
namespace platform
{

/*
 * Common platform implementations
 **********************************/
const vector<string>& get_environment_PATH()
{
  debug(debug::platform) << "platform::get_environment_PATH::Called.\n";
  static vector<string> result;
  if(!result.empty())
    return result;

#if _WIN32
  const std::string PATH = convert_to_utf8(_wgetenv(L"PATH"));
  const char delimiter = ';';
#else
  const std::string PATH = getenv("PATH");
  const char delimiter = ':';
#endif
  if(PATH.empty())
    throw runtime_error( "PATH should not be empty" );

  size_t previous = 0;
  size_t index = PATH.find(delimiter);
  while(index != string::npos)
  {
    result.push_back(PATH.substr(previous, index-previous));
    debug(debug::platform) << "platform::get_environment_PATH::part of PATH: " << result.back() << "\n";
    previous=index+1;
    index = PATH.find(delimiter, previous);
  }
  result.push_back(PATH.substr(previous));

  return result;
}

const string current_working_directory()
{
  //TODO: use Unicode API for Win32
  const size_t chunkSize=255;
  const size_t maxChunks=10240; // 2550 KiBs of current path are more than enough

  char stackBuffer[chunkSize]; // Stack buffer for the "normal" case
#ifdef _WIN32
  if(getcwd(stackBuffer, static_cast<int>(sizeof(stackBuffer))) != nullptr)
#else
  if(getcwd(stackBuffer, sizeof(stackBuffer)) != nullptr)
#endif
    return stackBuffer;
  if( errno!=ERANGE )
    throw std::runtime_error("Cannot determine the current path.");

  for(size_t chunks=2; chunks<maxChunks ; chunks++)
  {
    std::unique_ptr<char> cwd(new char[chunkSize*chunks]);
#if _WIN32
    if(getcwd(cwd.get(), static_cast<int>(chunkSize*chunks)) != nullptr)
#else
    if(getcwd(cwd.get(), chunkSize*chunks) != nullptr)
#endif
      return cwd.get();

    if(errno!=ERANGE)
      throw std::runtime_error("Cannot determine the current path.");
  }
  throw std::runtime_error("Cannot determine the current path; the path is apparently unreasonably long");
}
bool directory_exists(const string& directory)
{
#if _WIN32
  wstring directoryW(convert_to_utf16(directory));
  struct _stat64 status;
  if(_wstat64(directoryW.c_str(),& status) == 0)
#else // _WIN32
  struct stat status;
  if(stat(directory.c_str(), &status) == 0)
#endif //_WIN32
  {
    debug(debug::platform) << "platform::directory_exists::(_w)stat(64) succeeded for " << directory << ".\n";
    if (status.st_mode & S_IFDIR)
      return true;
  }
  debug(debug::platform) << "platform::directory_exists::" << directory << " is not a directory.\n";
  return false;
}
bool file_exists(const string& filename)
{
#if _WIN32
  wstring filenameW(convert_to_utf16(filename));
  struct _stat64 status;
  if(_wstat64(filenameW.c_str(), &status) == 0)
#else // _WIN32
  struct stat status;
  if(stat(filename.c_str(), &status) == 0)
#endif //_WIN32
  {
    debug(debug::platform) << "platform::file_exists::(_w)stat(64) succeeded.\n";
    if(status.st_mode & S_IFREG)
      return true;
  }
  debug(debug::platform) << "platform::file_exists::" << filename << " is not a file.\n";
  return false;
}
time_t last_modified(const std::string filename)
{
  struct stat attributes;
  if(stat(filename.c_str(), &attributes) == -1)
    return 0; //TODO: fix if not adequate on some obscure supported platform
  else
    return attributes.st_mtime;
}

// Find best available compiler
toolchain detect_toolchain()
{
  debug(debug::platform) << "platform::detect_toolchain::Detecting which toolchain is in PATH.\n";
  command toolchain_test;
  string std_out;
  string std_err;
  pair<bool, int> result;
#ifdef _WIN32
  // MSVC
  toolchain_test.set_program("cl");
  result = execute_command(toolchain_test, std_out, std_err);
  if(result.first)
    return toolchain::Microsoft;
  debug(debug::platform) << "platform::Failed to execute command \'cl\'.\n";

#endif
  toolchain_test.set_program("clang");
  toolchain_test.add_argument("--version");
  result = execute_command(toolchain_test, std_out, std_err);
  if(result.first)
    return toolchain::LLVM;

  toolchain_test.set_program("gcc");
  toolchain_test.add_argument("--version");
  result = execute_command(toolchain_test, std_out, std_err);
  if(result.first)
    return toolchain::GNU;
  debug(debug::platform) << "platform::Failed to execute \'gcc --version\' returned exit code " << result.second << ".\n";

  throw error("unable to detect a usable supported compiler.");
}

void create_directory_recursive(const string& name)
{
#ifdef _WIN32
  const wstring nameW = convert_to_utf16(name);
  const wchar_t* dir = nameW.c_str();
  wchar_t tmp[256];
  wchar_t *p = nullptr;
  const wchar_t dir_sep = L'/';
  const wchar_t* format_string = L"%s";
#else
  const char* dir = name.c_str();
  char tmp[256];
  char *p = nullptr;
  const char dir_sep = '/';
  const char* format_string = "%s";
#endif
  size_t len;

  snprintf(tmp, sizeof(tmp), format_string, dir);
#ifdef _WIN32
  len = wcslen(tmp);
#else
  len = strlen(tmp);
#endif
  if(tmp[len - 1] == dir_sep)
    tmp[len - 1] = 0;
  for(p = tmp + 1; *p; p++)
  {
    if(*p == dir_sep)
    {
      *p = 0;
      mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      *p = dir_sep;
    }
    mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
}

} // namespace platform

} // namespace lib

} // namespace ambrosia
