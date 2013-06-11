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
#include "Ambrosia/platform.h++"

// libAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/Error/error.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/typedefs.h++"

// Platform includes
#include <dirent.h>
#include <spawn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// C++ includes
#include <cstdlib>
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

namespace ambrosia
{
namespace lib
{
namespace platform
{

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

void command::set_program(const std::string& program_name)
{
  storage.clear();
  storage.push_back(program_name);
  array.pop_back();
  array.push_back(const_cast<char*>(storage.back().c_str()));
  array.push_back(nullptr);
}
void command::add_argument(const std::string& argument)
{
  storage.push_back(argument);
  array.pop_back(); // remove trailing nullptr
  array.push_back(const_cast<char*>(storage.back().c_str()));
  array.push_back(nullptr); // add trailing nullptr
}
void command::add_arguments(const command& other_command)
{
  array.pop_back(); // remove the last nullptr element
  for(auto&& command : other_command.storage)
  {
    storage.push_back(command); array.push_back(const_cast<char*>(storage.back().c_str()));
  }
  array.push_back(nullptr);
}

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
string_vector get_commandline_arguments(char *argv[],
                                        const int argc)
{
  return string_vector(argv+1, argv+argc);
}

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
template void scan_directory<insert_iterator<file_set>>(insert_iterator<file_set>, const string&);

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
        it = {directory_name / name, attributes.st_mtime};
    }
  }
  if(chdir(cwd.c_str()))
    throw runtime_error("Cannot access old working directory: " + cwd);

  closedir(dir);
}
// explicit instantiation
template void recursive_scan_directory<insert_iterator<file_set>>(insert_iterator<file_set>, const string&, const string&);

std::pair<bool, int> execute_command(const command &command,
                                     string &string_cout,
                                     string &string_cerr)
{
  // Create pipes for redirected output
  int cout_pipe[2];
  int cerr_pipe[2];
  if(pipe(cout_pipe) || pipe(cerr_pipe))
    throw error("pipe returned an error.");

  int exit_code;
  pid_t pid = fork();
  switch(pid)
  {
    case -1:
      throw error("fork failed.");
    case 0:
      debug(debug::command_exec, "unix::execute_command::Doing pipe stuff.\n");
      // Close read end in the child
      close(cout_pipe[0]);
      close(cout_pipe[0]);
      // Redirect stdout and stderr
      dup2(cout_pipe[1],1);
      dup2(cerr_pipe[1],2);
      // Close unneeded file descriptors
      close(cout_pipe[1]);
      close(cerr_pipe[1]);
      // execute child program
      if(execvp(command.array[0], &command.array[0]) == -1)
        std::_Exit(-1); // Quickly exit leaving the parent's file descriptors untouched.
    default:
      if(waitpid(pid, &exit_code, 0) != -1)
        debug(debug::command_exec, "unix::execute_command::Child exited with status ", exit_code, ".\n");
      else
        throw error("waitpid failed.");

      // Close pipes
      close(cout_pipe[1]);
      close(cerr_pipe[1]);
      // Read from pipes, a kB at a time
      const size_t buffer_size = 1024;
      string buffer;
      buffer.resize(buffer_size);
      ssize_t bytes_read = read(cout_pipe[0], &buffer[0], buffer_size);
      while ((bytes_read = read(cout_pipe[0], &buffer[0], buffer_size)) > 0)
      {
        string_cout.append(buffer.substr(0, static_cast<size_t>(bytes_read)+1));
        bytes_read = read(cout_pipe[0], &buffer[0], buffer_size);
      }
      if(bytes_read == -1)
        throw error("Failure reading from stdout pipe.");
      while ((bytes_read = read(cerr_pipe[0], &buffer[0], buffer_size)) > 0)
      {
        string_cerr.append(buffer.substr(0, static_cast<size_t>(bytes_read)+1));
        bytes_read = read(cout_pipe[0], &buffer[0], buffer_size);
      }
      if(bytes_read == -1)
        throw error("Failure reading from stdout pipe.");
  }
  debug(debug::command_exec, "unix::execute_command::Executed command with return code ", exit_code, ".\n");
  // TODO: figure out how to reliably detect if the exec call failed or the exec'ed program returned -1
  if(exit_code == -1)
    return std::make_pair(false,exit_code);
  else
    return std::make_pair(true,exit_code);
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

} // namespace platform

} // namespace lib

} // namespace ambrosia
