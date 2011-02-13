/**
  * Enums.h
  * Ambrosia's handy enums.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ENUMS_H
#define ENUMS_H

// Global include
#include "Ambrosia/Global.h"

namespace ambrosia
{
    enum class os
    {
        Windows,
        Linux,
        MacOSX
        // ...
    };
    enum class architecture
    {
        x86,
        amd64
        //...
    };
    enum class file_type
    {
        source,
        source_c,
        source_cxx,
        source_java,
        source_fortran,
        header,
        resource,
        Qt_ui,
        Qt_moc,
        Qt_rc
        // ...
    };
    enum class toolchain
    {
        GNU,
        Microsoft,
        LLVM,
        Intel
        // ...
    };
}

#endif // ENUMS_H
