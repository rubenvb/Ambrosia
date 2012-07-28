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

libambrosia_namespace_begin

// C-style languages
const command_map the_huge_command_map =
{
  {file_type::source_c,
    {
      {toolchain::GNU,
        {{generator_string::compiler,         "gcc"},
         {generator_string::linker,           "gcc"},
         {generator_string::output_argument,  "-o"},
         {generator_string::compile_argument, "-c"},
         {generator_string::linker_argument,  ""},
         {generator_string::object_suffix,    ".o"},
         {generator_string::debug_argument,   "-g"},
         {generator_string::optimize_none,    "-O0"},
         {generator_string::optimize_common,  "-fomit-frame-pointer -momit-leaf-frame-pointer"},
         {generator_string::optimize,         "-O2"},
         {generator_string::optimize_size,    "-Os"},
         {generator_string::optimize_extreme, "-O3"}
        }
      },
      {toolchain::Microsoft,
        {{generator_string::compiler,         "cl"},
         {generator_string::linker,           "link"},
         {generator_string::output_argument,  ""},
         {generator_string::compile_argument, "/c"},
         {generator_string::object_suffix,    ".obj"},
         {generator_string::debug_argument,   ""},
         {generator_string::optimize_none,    "/Od"},
         {generator_string::optimize,         "/O2"},
         {generator_string::optimize_size,    "/O1"},
         {generator_string::optimize_extreme, "/Ox"}
        }
      },
      {toolchain::LLVM,
        {{generator_string::compiler,         "clang"}
        }
      },
      {toolchain::Intel,
        {{generator_string::compiler,         "icc"}
        }
      }
    }
  },
  {file_type::source_cxx,
    {
      {toolchain::GNU,
        {{generator_string::compiler,         "g++"},
         {generator_string::linker,           "g++"},
         {generator_string::output_argument,  "-o"},
         {generator_string::compile_argument, "-c"},
         {generator_string::linker_argument,  ""},
         {generator_string::object_suffix,    ".o"},
         {generator_string::debug_argument,   "-g"},
         {generator_string::optimize_none,    "-O0"},
         {generator_string::optimize,         "-O2"},
         {generator_string::optimize_size,    "-Os"},
         {generator_string::optimize_extreme, "-O3"}
        }
      },
      {toolchain::Microsoft,
        {{generator_string::compiler,         "cl"},
         {generator_string::linker,           "link"},
         {generator_string::output_argument,  ""},
         {generator_string::compile_argument, "/c"},
         {generator_string::object_suffix,    ".obj"},
         {generator_string::debug_argument,   ""},
         {generator_string::optimize_none,    "/Od"},
         {generator_string::optimize,         "/O2"},
         {generator_string::optimize_size,    "/O1"},
         {generator_string::optimize_extreme, "/Ox"}
        }
      },
      {toolchain::LLVM,
        {{generator_string::compiler,         "clang"}
        }
      },
      {toolchain::Intel,
        {{generator_string::compiler,         "icc"}
        }
      }
    }
  }
};
const generator_map objcgenerator_map;
const generator_map objcxxgenerator_map;
libambrosia_namespace_end
