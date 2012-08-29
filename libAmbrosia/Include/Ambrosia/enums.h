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
  sub,
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
  linker, // linker program name
  output_argument, // precedes output file name
  compile_argument, // compile to object filew without linking
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

ambrosia_namespace_end

#endif // ENUMS_H
