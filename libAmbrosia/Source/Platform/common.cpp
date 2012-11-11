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

// Common platform includes
#if _WIN32
  #define S_IFREG _S_IFREG
  #include <direct.h>
  #define getcwd _getcwd
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
#include <memory>
  using std::unique_ptr;
#include <stdexcept>
  using std::runtime_error;
#include <string>
  using std::string;
  using std::wstring;
#include <vector>
  using std::vector;

libambrosia_namespace_begin

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

} // namespace platform

libambrosia_namespace_end
