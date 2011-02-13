/**
  * Global.h
  * Global header included by all other Ambrosia library headers.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef GLOBAL_LIBRARY_H
#define GLOBAL_LIBRARY_H

namespace ambrosia
{
    // Versioning
    const int lib_version_major = 0;
    const int lib_version_minor = 0;
    const int lib_version_bugfix = 1;
}

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
