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
 * Ambrosia/Generators/generator_maps.h
 * Command building blocks for all toolchains/languages.
 *
 **/

#ifndef AMBROSIA_GENERATORS_GENERATOR_MAPS_H
#define AMBROSIA_GENERATORS_GENERATOR_MAPS_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

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
  include_option, // -I, /I
  output_object, // -o, /Fo
  output_pch, // -o, /Fp
  output_debug, //
  compile_option, // -c, /c
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

  static_library_extension, // .a, .lib
  import_library_extension, // .dll.a .dll.lib?
  shared_library_extension, // .so, .dll, .dylib

};
typedef std::map<toolchain_option, std::string> toolchain_option_map;
extern const std::map<toolchain, toolchain_option_map> toolchain_options;

// Language-specific options
extern const std::map<file_type, language_option_map> language_options;

// OS-specific bits
extern const std::map<os, os_bits_map> os_bits;

// map to map the maps according to file_type
extern const command_map the_huge_command_map;

libambrosia_namespace_end

#endif // AMBROSIA_GENERATORS_GENERATOR_MAPS_H
