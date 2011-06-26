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
#if _WIN32
    const std::string convert_to_utf8( const std::wstring &utf16_string );
    const std::wstring convert_to_utf16( const std::string &utf8_string );
    inline const std::wstring transform_filename( const std::string &filename )
    { return convert_to_utf16(filename); }
#else // _WIN32
    inline const std::string transform_filename( const std::string &filename )
    { return filename; }
#endif // _WIN32

// Single level directory scan
template<class output_iterator>
void scan_directory( output_iterator it, const std::string &relative_directory );
// Recursive directory listing with generic output iterator
template<class output_iterator>
void recursive_scan_directory( output_iterator it, const std::string &relative_directory,
                               const std::string &directory_name = "" );

libambrosia_namespace_end

#endif // PLATFORM_H
