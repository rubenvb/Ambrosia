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

libambrosia_namespace_begin

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
  amd64,
  ARM
  // ...
};
// Toolchain comprising of compiler, linker, and general tools
enum class toolchain
{
  GNU, // GCC+binutils
  Microsoft, // cl.exe and link.exe
  LLVM, // Clang+binutils
  Intel // ICC+platform linker
  // ...
};
// Shell execution environment
enum class environment
{
  cmd, // Windows' cmd.exe
  sh // POSIX shell
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
  application,
  library,
  project,
  install,
  test,
  all
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

// Toolchain-specific options
enum class toolchain_option
{
  compiler_c, // gcc, clang, cl, icc
  compiler_cxx, // g++ clang++, cl, icc
  compiler_fortran, // gfortran, ...

  include_dir, // -I, /I
  include_file, // -include, /FI
  include_pch, // -include, -Fp
  include_sysfile, // -sys-include

  output_object, // -o, /Fo
  output_pch, // -o, /Fp
  output_debug, //
  output_import_library, // /implib,

  compile_only, // -c, /c
  compile_debug, // -g, /Zi

  object_extension, // .o, .obj

  optimize_none, // -O0, /Od
  optimize_normal, // -O2, /O2
  optimize_size, // -Os, /O1
  optimize_extreme, // -O3, /Ox
  optimize_link, // -flto, /GL
  optimize_extra, // options that make debugging impossible but added because Ambrosia is awesome
                  // these include for example: -fomit-frame-pointer, -momit-leaf-frame-pointer, /Gy

  enable_sse, // -msse, /arch:SSE
  enable_sse2, // -msse2, /arch:SSE2

  dynamic_linker_c, // executable and shared library linker for C
  dynamic_linker_cxx, // executable and shared library linker for C++
  dynamic_linker_fortran, // executable and shared library linker for Fortran

  static_linker, // static library archive linker driver
  static_link_options, // default static linker options

  link_debug, // /DEBUG
  link_optimize, // -flto, /LTCG /NOWIN98
  link_strip, // -s, /OPT:ICF
  link_library, // -l
  link_search_directory, // -L, /libpath:

  runtime_library_cxx, // libstdc++, libc++
  runtime_library_fortran, // libgfortran, ...

  static_library_prefix, // lib
  static_library_extension, // .a, .lib
  shared_library_prefix, // lib
  import_library_extension // .dll.a .dll.lib
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

libambrosia_namespace_end

#endif // ENUMS_H
