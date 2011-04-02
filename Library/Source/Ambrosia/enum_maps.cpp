/**
  * Ambrosia/enum_maps.cpp
  * Enum map definitions.
  *
  * Author: Ruben Van Boxem
  *
  **/

// map declarations
#include "enum_maps.h"

// C++ includes
/* <map> */
    using std::map;
/* <string> */
    using std::string;

namespace ambrosia
{
    const map<string, architecture> architecture_map =
             { {"x86",   architecture::x86},
               {"amd64", architecture::amd64} };

    const map<string, os> os_map =
             { {"windows", os::Windows},
               {"linux",   os::Linux},
               {"mac",     os::MacOSX} };

    // toolchain has two maps: one for the ambrosia cross specification,
    //  another for true vendor name output. Both map to the same internal
    //  representation (i.e. the vendor name).
    const map<string, toolchain> vendor_map =
             { {"GNU",       toolchain::GNU},
               {"Microsoft", toolchain::Microsoft},
               {"LLVM",      toolchain::LLVM},
               {"Intel",     toolchain::Intel} };
    const map<string, toolchain> toolchain_map =
             { {"gcc",   toolchain::GNU},
               {"msvc",  toolchain::Microsoft},
               {"clang", toolchain::LLVM},
               {"icc",   toolchain::Intel} };
} // namespace ambrosia

