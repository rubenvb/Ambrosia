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
 * Ambrosia/Generators/generator_maps.cpp
 * Generator map definitions.
 *
 **/

// Map declarations
#include "Ambrosia/Generators/generator_maps.h"

// libAmbrosia includes
#include "Ambrosia/boost_wrapper.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// C-style languages
/*const command_map the_huge_command_map =
  map_entries_begin
    entry_begin file_type::source_c,
      map_entries_begin
        entry_begin toolchain::GNU,
          map_entries_begin
            entry_begin generator_string::compiler,          "gcc" entry_end
            entry_begin generator_string::dynamic_linker,    "gcc" entry_end
            entry_begin generator_string::static_linker,     "ar" entry_end
            entry_begin generator_string::output_object  ,   "-o" entry_end
            entry_begin generator_string::output_executable, "-o" entry_end
            entry_begin generator_string::output_static_lib, "" entry_end
            entry_begin generator_string::output_shared_lib, "-o" entry_end
            entry_begin generator_string::output_import_lib, "-Wl,--out-implib" entry_end
            entry_begin generator_string::compile_argument,  "-c" entry_end
            entry_begin generator_string::compile_shared,    "" entry_end
            entry_begin generator_string::compile_static,    "" entry_end
            entry_begin generator_string::include_argument,  "-I" entry_end
            entry_begin generator_string::object_suffix,     ".o" entry_end
            entry_begin generator_string::debug_argument,    "-g" entry_end
            entry_begin generator_string::optimize_none,     "-O0" entry_end
            entry_begin generator_string::optimize_common,   "-fomit-frame-pointer -momit-leaf-frame-pointer" entry_end
            entry_begin generator_string::optimize,          "-O2" entry_end
            entry_begin generator_string::optimize_size,     "-Os" entry_end
            entry_begin generator_string::optimize_extreme,  "-O3" entry_end
          entries_end
        entry_end
        entry_begin toolchain::Microsoft,
          map_entries_begin
            entry_begin generator_string::compiler,          "cl" entry_end
            entry_begin generator_string::dynamic_linker,    "link" entry_end
            entry_begin generator_string::static_linker,     "lib" entry_end
            entry_begin generator_string::output_object  ,   "/Fo" entry_end
            entry_begin generator_string::output_executable, "/OUT" entry_end
            entry_begin generator_string::output_static_lib, "" entry_end
            entry_begin generator_string::output_shared_lib, "/DLL /OUT" entry_end
            entry_begin generator_string::output_import_lib, "/IMPLIB" entry_end
            entry_begin generator_string::compile_argument,  "/c" entry_end
            entry_begin generator_string::compile_shared,    "/MD" entry_end
            entry_begin generator_string::compile_static,    "/MT" entry_end
            entry_begin generator_string::include_argument,  "/I" entry_end
            entry_begin generator_string::object_suffix,     ".obj" entry_end
            entry_begin generator_string::compile_debug,     "/Zi" entry_end
            entry_begin generator_string::link_debug,     "/DEBUG" entry_end
            entry_begin generator_string::optimize_none,     "/Od" entry_end
            entry_begin generator_string::optimize,          "/O2" entry_end
            entry_begin generator_string::optimize_size,     "/O1" entry_end
            entry_begin generator_string::optimize_extreme,  "/Ox" entry_end
          entries_end
        entry_end
        entry_begin toolchain::LLVM,
          map_entries_begin
            entry_begin generator_string::compiler,         "clang" entry_end
            entry_begin generator_string::linker,           "clang" entry_end
            entry_begin generator_string::output_argument,  "-o" entry_end
            entry_begin generator_string::compile_argument, "-c" entry_end
            entry_begin generator_string::include_argument, "-I" entry_end
            entry_begin generator_string::object_suffix,    ".o" entry_end
            entry_begin generator_string::debug_argument,   "-g" entry_end
            entry_begin generator_string::optimize_none,    "-O0" entry_end
            entry_begin generator_string::optimize,         "-O2" entry_end
            entry_begin generator_string::optimize_size,    "-Os" entry_end
            entry_begin generator_string::optimize_extreme, "-O3" entry_end
          entries_end
        entry_end
        entry_begin toolchain::Intel,
          map_entries_begin
            entry_begin generator_string::compiler,         "icc" entry_end
          entries_end
        entry_end
      entries_end
    entry_end
    entry_begin file_type::source_cxx,
      map_entries_begin
       entry_begin toolchain::GNU,
         map_entries_begin
            entry_begin generator_string::compiler,         "g++" entry_end
            entry_begin generator_string::linker,           "g++" entry_end
            entry_begin generator_string::output_argument,  "-o" entry_end
            entry_begin generator_string::compile_argument, "-c" entry_end
            entry_begin generator_string::include_argument, "-I" entry_end
            entry_begin generator_string::object_suffix,    ".o" entry_end
            entry_begin generator_string::debug_argument,   "-g" entry_end
            entry_begin generator_string::optimize_none,    "-O0" entry_end
            entry_begin generator_string::optimize,         "-O2" entry_end
            entry_begin generator_string::optimize_size,    "-Os" entry_end
            entry_begin generator_string::optimize_extreme, "-O3" entry_end
          entries_end
        entry_end
        entry_begin toolchain::Microsoft,
          map_entries_begin
            entry_begin generator_string::compiler,         "cl" entry_end
            entry_begin generator_string::linker,           "link" entry_end
            entry_begin generator_string::output_argument,  "/o" entry_end
            entry_begin generator_string::compile_argument, "/c" entry_end
            entry_begin generator_string::include_argument, "/I" entry_end
            entry_begin generator_string::object_suffix,    ".obj" entry_end
            entry_begin generator_string::debug_argument,   "" entry_end
            entry_begin generator_string::optimize_none,    "/Od" entry_end
            entry_begin generator_string::optimize,         "/O2" entry_end
            entry_begin generator_string::optimize_size,    "/O1" entry_end
            entry_begin generator_string::optimize_extreme, "/Ox" entry_end
          entries_end
        entry_end
        entry_begin toolchain::LLVM,
          map_entries_begin
            entry_begin generator_string::compiler,         "clang++" entry_end
            entry_begin generator_string::linker,           "clang++" entry_end
            entry_begin generator_string::output_argument,  "-o" entry_end
            entry_begin generator_string::compile_argument, "-c" entry_end
            entry_begin generator_string::include_argument, "-I" entry_end
            entry_begin generator_string::object_suffix,    ".o" entry_end
            entry_begin generator_string::debug_argument,   "-g" entry_end
            entry_begin generator_string::optimize_none,    "-O0" entry_end
            entry_begin generator_string::optimize,         "-O2" entry_end
            entry_begin generator_string::optimize_size,    "-Os" entry_end
            entry_begin generator_string::optimize_extreme, "-O3" entry_end
          entries_end
        entry_end
        entry_begin toolchain::Intel,
          map_entries_begin
            entry_begin generator_string::compiler,         "icc" entry_end
          entries_end
        entry_end
      entries_end
    entry_end
  entries_end*/

libambrosia_namespace_end
