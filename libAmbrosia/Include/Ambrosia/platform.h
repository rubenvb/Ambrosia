/**
  * Ambrosia/platform.h
  * Ambrosia's platform dependent functions.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_PLATFORM_H
#define AMBROSIA_PLATFORM_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"

// C++ includes
#include <memory>
#include <string>
#include <vector>

libambrosia_namespace_begin

/*
 * constants
 *********************/
extern const char directory_seperator;
extern const std::string executable_suffix;
extern const os build_os;
extern const architecture build_architecture;
extern const architecture ambrosia_architecture;

/*
 * Windows support functions
 ****************************/
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

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
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

libambrosia_namespace_end

#endif // AMBROSIA_PLATFORM_H
