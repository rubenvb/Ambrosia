/**
  * Ambrosia/platform.h
  * Ambrosia's platform dependent functions.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PLATFORM_H
#define PLATFORM_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "enums.h"

// C++ include
#include <string>

libambrosia_namespace_begin

/*
 * constants
 *********************/
extern const char directory_seperator;
extern const std::string executable_suffix;
extern const os build_os;
extern const architecture build_architecture;
/*
 * Possible commonly implemented functions
 ******************************************/
// current working directory in string form.
const std::string current_working_directory();
// check if a file/directory exists/is accessible
bool directory_exists( const std::string &directory );
bool file_exists( const std::string &filename );

/*
 * Mostly platform dependently implemented functions
 ****************************************************/
// Single level directory scan
template<class output_iterator>
void scan_directory( output_iterator it, const std::string &relative_directory );
// Recursive directory listing with generic output iterator
template<class output_iterator>
void recursive_scan_directory( output_iterator it, const std::string &relative_directory,
                               const std::string &directory_name = "" );

/*
 * Ugly workarounds
 *******************/
#if _WIN32
# if __GLIBCXX__
    std::unique_ptr<std::istream> open_ifstream( const std::string &filename );
    std::unique_ptr<std::ostream> open_ofstream( const std::string &filename );
# endif
#else
    std::unique_ptr<std::ifstream> open_ifstream( const std::string &filename );
    std::unique_ptr<std::ofstream> open_ofstream( const std::string &filename );
#endif

libambrosia_namespace_end

#endif // PLATFORM_H
