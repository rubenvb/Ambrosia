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
 * Ambrosia/platform.h
 * Ambrosia's platform dependent functions.
 *
 **/

#ifndef AMBROSIA_PLATFORM_H
#define AMBROSIA_PLATFORM_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ambrosia
{
namespace lib
{
namespace platform
{

/*
 * constants
 *********************/
extern const char directory_seperator;
extern const std::string executable_suffix;
extern const os build_os;
extern const architecture build_architecture;
extern const architecture ambrosia_architecture;
const toolchain ambrosia_toolchain =
#ifdef __clang__
  toolchain::LLVM;
#elif defined(__ICL)
  toolchain::Intel;
#elif defined(_MSC_VER)
  toolchain::Microsoft;
#elif defined(__GNUC__)
  toolchain::GNU;
#endif
const toolchain default_toolchain =
#ifdef _WIN32
  toolchain::GNU;
#elif defined(__linux__)
  toolchain::GNU;
#elif defined(__APPLE__)
  toolchain::LLVM;
#endif

/*
 * Child process command struct
 *******************************/
struct command
{
  void set_program(const std::string& program_name);
  void add_argument(const std::string& argument);
  void add_arguments(const command& other_command);
#if _WIN32
  mutable std::wstring arguments; // mutable cause CreateProcessW likes to modify its arguments
#else
  command() : array{nullptr}, storage()
  {   }
  std::vector<char*> array; // &arguments[0] is convertible to char*[] for the call to execvp
  std::vector<std::string> storage;
#endif
};

/*
 * Platform support functions
 *****************************/
#if _WIN32
const std::string convert_to_utf8(const std::wstring& utf16_string);
const std::wstring convert_to_utf16(const std::string& utf8_string);
#endif // _WIN32
/*
 * Possible commonly implemented functions
 ******************************************/
// PATH environment variable
const std::vector<std::string>& get_environment_PATH();
// current working directory in string form.
const std::string current_working_directory();
// check if a file/directory exists/is accessible
bool directory_exists(const std::string& directory);
bool file_exists(const std::string& filename);
// Get last modified time of file
time_t last_modified(const std::string filename);
// Find best available compiler
toolchain detect_toolchain();

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
string_vector get_commandline_arguments(char*[], const int);
bool is_absolute_path(const std::string& path);
// Single level directory scan
template<class output_iterator>
void scan_directory(output_iterator it,
                    const std::string& relative_directory);
// Recursive directory listing with generic output iterator
template<class output_iterator>
void recursive_scan_directory(output_iterator it,
                              const std::string& relative_directory,
                              const std::string& directory_name = "" );
// Create a directory, with all parent directories
void create_directory_recursive(const std::string& name);
// Execute command, and store stdout and stderr output in respective strings
std::pair<bool, int> execute_command(const platform::command& command,
                                     std::string& std_out,
                                     std::string& std_err);

/*
 * Ugly workarounds
 *******************/
#if defined(_WIN32) && defined(__GLIBCXX__)
  std::unique_ptr<std::istream> open_ifstream(const std::string& filename);
  std::unique_ptr<std::ostream> open_ofstream(const std::string& filename);
#else
  std::unique_ptr<std::ifstream> open_ifstream(const std::string& filename);
  std::unique_ptr<std::ofstream> open_ofstream(const std::string& filename);
#endif

} // namespace platform

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_PLATFORM_H
