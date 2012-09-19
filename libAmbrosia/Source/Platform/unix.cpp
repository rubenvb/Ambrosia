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
 * Ambrosia/Platform/linux.cpp
 * Function implementations for GNU/Linux.
 *
 **/

// Function include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// Platform includes
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

// C++ includes
#include <fstream>
  using std::ifstream;
  using std::ofstream;
#include <iterator>
  using std::back_insert_iterator;
  using std::insert_iterator;
#include <memory>
  using std::unique_ptr;
#include <stdexcept>
  using std::runtime_error;
#include <string>
  using std::string;
#include <vector>
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
#if __i386__ // this works for GCC
  const architecture build_architecture = architecture::x86;
#elif __x86_64__
  const architecture build_architecture = architecture::amd64;
#endif // i386 || __x86_64__

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
bool is_absolute_path(const string& path)
{
  if(path.empty())
    return false;
  else
    return (path[0] == '/');
}

template<class output_iterator>
void scan_directory(output_iterator it,
                    const string& directory_name)
{
  DIR* dir = opendir(directory_name.c_str());

  if(dir == 0)
    throw runtime_error("unable to open file: " + directory_name);

  // store cwd to return to original directory when finished
  string cwd(current_working_directory());

  if(chdir(directory_name.c_str()))
    throw runtime_error("Cannot access directory: " + directory_name);

  for(dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
  {
    struct stat attributes;
    const string name = entry->d_name;
    if(stat(name.c_str(), &attributes) == -1)
      throw runtime_error("stat failed...");

    if(S_ISREG(attributes.st_mode))
      it = {name, attributes.st_mtime};
  }
  if(chdir(cwd.c_str()))
    throw runtime_error("Cannot access old working directory: " + cwd);

  closedir(dir);
}
template void scan_directory<insert_iterator<file_set> >(insert_iterator<file_set>, const string&);

template<class output_iterator>
void recursive_scan_directory(output_iterator it,
                              const string& relative_directory,
                              const string& directory_name)
{
  DIR* dir = opendir(relative_directory.c_str());

  if(dir == 0)
    throw runtime_error("Unable to open file: " + directory_name);

  // store cwd to return to original directory when finished
  string cwd(current_working_directory());

  if(chdir(relative_directory.c_str()))
    throw runtime_error("Directory cannot be accessed: " + relative_directory);

  for(dirent* entry = readdir(dir); entry != 0; entry = readdir(dir))
  {
    struct stat attributes;
    const string name = entry->d_name;
    if(stat(name.c_str(), &attributes) == -1)
      throw runtime_error("stat failed...");

    if(S_ISDIR(attributes.st_mode))
    {
      // skip ".", "..", and ".*" (hidden directory on *nix)
      if(strcmp(".",name.substr(0,1).c_str()) == 0)
        continue;
      else
        recursive_scan_directory(it, name, name);
    }
    else
    {
      if(directory_name.empty())
        it = {name, attributes.st_mtime};
      else
        it = {directory_name + "/" + name, attributes.st_mtime};
    }
  }
  if(chdir(cwd.c_str()))
    throw runtime_error("Cannot access old working directory: " + cwd);

  closedir(dir);
}
// explicit instantiation
template void recursive_scan_directory<insert_iterator<file_set> >(insert_iterator<file_set>, const string&, const string&);

bool create_directory(const string &name)
{
  // from simple example from OpenGroup docs
  bool result = mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  return result;
}

/*
 * Ugly workarounds
 *******************/
unique_ptr<ifstream> open_ifstream(const std::string& filename)
{
  return unique_ptr<ifstream>(new ifstream(filename));
}
unique_ptr<ofstream> open_ofstream(const std::string& filename)
{
  return unique_ptr<ofstream>(new ofstream(filename));
}

libambrosia_namespace_end
