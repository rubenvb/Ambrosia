/**
  * Ambrosia/Generators/generator_maps.cpp
  * Generator map definitions.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Map declarations
#include "Ambrosia/Generators/generator_maps.h"

libambrosia_namespace_begin

// C-style languages
const generator_map cgenerator_map =
         { { toolchain::GNU,
             { {generator_string::compiler,         "gcc"},
               {generator_string::linker,           "gcc"},
               {generator_string::output_argument,  "-o"},
               {generator_string::compile_argument, "-c"},
               {generator_string::linker_argument,  ""},
               {generator_string::object_suffix,    ".o"},
               {generator_string::debug_argument,   " -g"},
               {generator_string::optimize_none,    "-O0"},
               {generator_string::optimize,         "-O2"},
               {generator_string::optimize_size,    "-Os"},
               {generator_string::optimize_extreme, "-O3"} } },
           { toolchain::Microsoft,
             { {generator_string::compiler,         "cl"},
               {generator_string::linker,           "link"},
               {generator_string::output_argument,  ""},
               {generator_string::compile_argument, "/c"},
               {generator_string::object_suffix,    ".obj"},
               {generator_string::debug_argument,   ""},
               {generator_string::optimize_none,    "/Od"},
               {generator_string::optimize,         "/O2"},
               {generator_string::optimize_size,    "/O1"},
               {generator_string::optimize_extreme, "/Ox"} } },
           { toolchain::LLVM,
             { {generator_string::compiler,         "clang"} } },
           { toolchain::Intel,
             { {generator_string::compiler,         "icc"} } } };
extern const generator_map cxxgenerator_map;
extern const generator_map objcgenerator_map;
extern const generator_map objcxxgenerator_map;
libambrosia_namespace_end
