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
 * Ambrosia/enums.h
 * Ambrosia's handy enums.
 *
 **/

#ifndef AMBROSIA_ENUMS_H
#define AMBROSIA_ENUMS_H

// Global include
#include "Ambrosia/global.h"

ambrosia_namespace_begin

// Operating system
enum class os
{
  Windows,
  Linux,
  MacOSX
    // ...
};
// CPU architecture
enum class architecture
{
  x86,
  amd64
  //...
};
// Toolchain comprising of compiler, linker, and general tools
enum class toolchain
{
  GNU, // GCC+GNU binutils
  Microsoft, // cl.exe and link.exe
  LLVM, // Clang+GNU binutils (may change later)
  Intel // ICC+platform linker
  // ...
};
// Shell execution environment
enum class environment
{
  cmd, // Windows' cmd.exe
  bash // GNU Bash or compatible
  // ...
};

enum class conditional_operator
{   // defines operator precedence
  right_parenthesis, // )
  left_parenthesis,  // (
  and_op,            // +
  or_op,             // |
  not_op             // !
};
enum class target_type
{
  global,
  app,
  lib,
  project,
  install,
  test
};
enum class file_type
{
  source,
  source_c,
  source_cxx,
  source_java,
  source_fortran,
  header,
  header_c,
  header_cxx,
  library,
  resource,
  Qt_ui,
  Qt_moc,
  Qt_rc
  // ...
};

enum class generator_string
{
  compiler, // compiler program name (without target prefixes)
  dynamic_linker, // linker program name (for executables and shared libraries)
  static_linker, // linker program name (for static library archives)
  output_object, // precedes object filename
  output_executable, // precedes executable filename
  output_static_lib, // precedes static library filename
  output_shared_lib, // precedes shared library filename
  output_import_lib, // precedes import library filename
  output_pch, // precedes precompiled header filename
  compile_argument, // compile to object filew without linking
  compile_shared, // compile to be linked dynamically to runtime libraries
  compile_static, // compile to be linked statically to runtime libraries
  include_argument, // precedes extra include directory
  object_suffix, // suffix for intermediate object files
  import_library_suffix, // .dll.a, .lib
  static_library_suffix, // .a, .lib
  shared_library_suffix, // .dll, .so
  debug_argument, // produce debug info
  optimize, // standard "release" configuration optimization
  optimize_none, // explicitely turn off optimizations
  optimize_common, // options that make debugging impossible but added because Ambrosia is awesome
  optimize_size, // optimize for size
  optimize_extreme, // full optimization, not guaranteed to be better than 'optimize_release'
  optimize_lto // Link-time-optimization or link time code generation: -flto or /GL and /LTCG
};

// Toolchain-specific options
enum class toolchain_option
{
  compiler, // gcc, clang, cl, icc

  include_dir, // -I, /I
  include_file, // -include?
  include_sysfile, // -sys-include?

  output_object, // -o, /Fo
  output_pch, // -o, /Fp
  output_debug, //

  compile_only, // -c, /c
  compile_debug, // -g, /Zi

  object_extension, // .o, .obj

  optimize_none, // -O0, /Od
  optimize_normal, // -O2, /O2
  optimize_size, // -Os, /O1
  optimize_extreme, // -O3, /Ox
  optimize_link, // -flto, /GL
  optimize_extra, // options that make debugging impossible but added because Ambrosia is awesome
                  // these include for example: -fomit-frame-pointer, -momit-leaf-frame-pointer, /Gy,
  dynamic_linker, // executable and shared library linker driver
  link_debug, // /DEBUG
  link_optimize, // -flto, /LTCG /NOWIN98
  link_strip, // -s, /OPT:ICF

  static_library_prefix, // lib
  static_library_extension, // .a, .lib
  shared_library_prefix, // lib
  import_library_extension, // .dll.a .dll.lib

};
// Language-specific options
enum class language_option
{
  compile_language, // -x [objective-]c[++]

  std_c89,
  std_c90,
  std_c99,
  std_c11,

  std_gnu89,
  std_gnu90,
  std_gnu99,
  std_gnu11,

  std_cxx98,
  std_cxx03,
  std_cxx11,

  std_gnuxx98,
  std_gnuxx03,
  std_gnuxx11
};
// OS-specific options
enum class os_option
{
  executable_extension,
  shared_library_extension
};

ambrosia_namespace_end

#endif // ENUMS_H
