/**
  * Ambrosia/global.h
  * Global header included by all other Ambrosia library headers.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef GLOBAL_LIBRARY_H
#define GLOBAL_LIBRARY_H

// namespace definition
#define ambrosia_namespace_begin namespace ambrosia {
#define ambrosia_namespace_end }
#define libambrosia_namespace_begin namespace ambrosia { namespace lib {
#define libambrosia_namespace_end } }

libambrosia_namespace_begin

// Versioning
const int version_major = 0;
const int version_minor = 0;
const int version_bugfix = 1;

// global run build configuration
class build_config;
extern build_config s_build_config;

libambrosia_namespace_end

#include "build_config.h"

// namespace shortcut
namespace libambrosia = ambrosia::lib;

// Windows DLL magic
#if defined(AMBROSIA_DLL)
# if defined(AMBROSIA_BUILD_DLL)
#  define DLL_EXPORT  __declspec(dllexport)
# else // AMBROSIA_BUILD_DLL
#  define DLL_EXPORT  __declspec(dllimport)
# endif // AMBROSIA_BUILD_DLL
#else // AMBROSIA_DLL
# define DLL_EXPORT
#endif // AMBROSIA_DLL

#endif // GLOBAL_H
