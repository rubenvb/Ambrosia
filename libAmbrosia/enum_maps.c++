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
 * enum_maps.cpp
 * Enum map definitions.
 *
 **/

// map declarations
#include "Ambrosia/enum_maps.h++"

// libAmbrosia includes

// C++ includes
#include <functional>
  using std::binary_function;
#include <map>
  using std::map;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

const map<string, architecture> architecture_map =
  {
    {"x86",   architecture::x86},
    {"amd64", architecture::amd64}
  };
const map<architecture, string> architecture_map_inverse =
  {
    {architecture::x86,   "x86"},
    {architecture::amd64, "AMD64"}
  };

// libambrosia::os has two maps: one for config related stuff, the other for nice output.
const map<os, string> os_name_map =
  {
    {os::Windows, "Windows"},
    {os::Linux,   "Linux"},
    {os::MacOSX,  "Mac OS X"}
  };
const map<string, os> os_name_map_inverse =
  {
    {"Windows",  os::Windows},
    {"Linux",    os::Linux},
    {"Mac OS X", os::MacOSX}
  };
const map<string, os> os_map =
  {
    {"windows", os::Windows},
    {"linux",   os::Linux},
    {"mac",     os::MacOSX}
  };
const map<os, string> os_map_inverse =
  {
    {os::Windows, "windows"},
    {os::Linux,   "linux"},
    {os::MacOSX,  "mac"}
  };

// libambrosia::toolchain has two maps: one for the ambrosia cross specification, another for true vendor name output. Both map to the same internal representation (i.e. the vendor name).
const map<string, toolchain> vendor_map =
  {
    {"GNU",       toolchain::GNU},
    {"Microsoft", toolchain::Microsoft},
    {"LLVM",      toolchain::LLVM},
    {"Intel",     toolchain::Intel}
  };
const map<toolchain, string> vendor_map_inverse =
  {
    {toolchain::GNU,       "GNU"},
    {toolchain::Microsoft, "Microsoft"},
    {toolchain::LLVM,      "LLVM"},
    {toolchain::Intel,     "Intel"}
  };

const map<string, toolchain> toolchain_map =
  {
    {"gcc",   toolchain::GNU},
    {"msvc",  toolchain::Microsoft},
    {"clang", toolchain::LLVM},
    {"icc",   toolchain::Intel}
  };
const map<toolchain, string> toolchain_map_inverse =
  {
    {toolchain::GNU,       "gcc"},
    {toolchain::Microsoft, "msvc"},
    {toolchain::LLVM,      "clang"},
    {toolchain::Intel,     "icc"}
  };

const map<string, target_type> target_type_map =
  {
    {"app",     target_type::application},
    {"lib",     target_type::library},
    {"sub",     target_type::project},
    {"install", target_type::install},
    {"test",    target_type::test}
  };
const map<target_type, string> target_type_map_inverse =
  {
    {target_type::application, "application"},
    {target_type::library,     "library"},
    {target_type::project,     "subproject"},
    {target_type::install,     "install"},
    {target_type::test,        "test"}
  };

const map<string, file_type> file_type_map =
  {
    {"SOURCES",         file_type::source},
    {"SOURCES_C",       file_type::source_c},
    {"SOURCES_CXX",     file_type::source_cxx},
    {"SOURCES_JAVA",    file_type::source_java},
    {"SOURCES_FORTRAN", file_type::source_fortran},
    {"HEADERS",         file_type::header},
    {"HEADERS_C",       file_type::header_c},
    {"HEADERS_CXX",     file_type::header_cxx},
    {"OBJECT",          file_type::object},
    {"LIBS",            file_type::library},
    {"RESOURCES",       file_type::resource},
    {"QT_UI",           file_type::Qt_ui},
    {"QT_MOC",          file_type::Qt_moc},
    {"QT_RC",           file_type::Qt_rc}
  };
const map<file_type, string> file_type_map_inverse =
  {
    {file_type::source,         "source"},
    {file_type::source_c,       "C source"},
    {file_type::source_cxx,     "C++ source"},
    {file_type::source_java,    "Java source"},
    {file_type::source_fortran, "Fortran source"},
    {file_type::header,         "header"},
    {file_type::header_c,       "C header"},
    {file_type::header_cxx,     "C++ header"},
    {file_type::object,         "object"},
    {file_type::library,        "library"},
    {file_type::resource,       "resource"},
    {file_type::Qt_ui,          "Qt ui"},
    {file_type::Qt_moc,         "Qt moc"},
    {file_type::Qt_rc,          "Qt rc"}
  };
const map<string, file_type> directory_type_map =
  {
    {"SOURCE_DIRS",   file_type::source},
    {"HEADER_DIRS",   file_type::header},
    {"RESOURCE_DIRS", file_type::resource},
    {"QT_UI_DIRS",    file_type::Qt_ui},
    {"QT_MOC_DIRS",   file_type::Qt_moc},
    {"QT_RC_DIRS",    file_type::Qt_rc}
  };
const map<file_type, string> directory_type_map_inverse =
  {
    {file_type::source,   "source"},
    {file_type::header,   "header"},
    {file_type::resource, "resource"},
    {file_type::Qt_ui,    "Qt ui"},
    {file_type::Qt_moc,   "Qt moc"},
    {file_type::Qt_rc,    "Qt rc"}
  };

const std::map<std::string, build_type> build_type_map =
  {
    {"release",            build_type::release},
    {"debug",              build_type::debug},
    {"release_with_debug", build_type::release_with_debug},
    {"profile",            build_type::profile},
  };
extern const std::map<build_type, std::string> build_type_map_inverse =
  {
    {build_type::release,            "release"},
    {build_type::debug,              "debug"},
    {build_type::release_with_debug, "release with debug information"},
    {build_type::profile,            "profile"},
  };
extern const std::map<std::string, linkage> linkage_map =
  {
    {"shared", linkage::load_time},
    {"static", linkage::link_time}
  };
extern const std::map<linkage, std::string> linkage_map_inverse =
  {
    {linkage::load_time, "shared"},
    {linkage::link_time, "static"}
  };

const map<string, conditional_operator> conditional_operator_map =
  {
    {")", conditional_operator::right_parenthesis},
    {"(", conditional_operator::left_parenthesis},
    {"+", conditional_operator::and_op},
    {"|", conditional_operator::or_op},
    {"!", conditional_operator::not_op}
  };
const map<conditional_operator, std::string> conditional_operator_map_inverse =
  {
    {conditional_operator::right_parenthesis, ")"},
    {conditional_operator::left_parenthesis,  "("},
    {conditional_operator::and_op,            "+"},
    {conditional_operator::or_op,             "|"},
    {conditional_operator::not_op,            "!"}
  };

const std::map<toolchain, toolchain_option_map> toolchain_options =
  {
    {toolchain::GNU,
      {
        {toolchain_option::compiler_c,       "gcc"},
        {toolchain_option::compiler_cxx,     "g++"},
        {toolchain_option::compiler_fortran, "gfortran"},
        {toolchain_option::compiler_nologo,  ""},

        {toolchain_option::define, "-D"},

        {toolchain_option::include_dir,     "-I"},
        {toolchain_option::include_file,    "-include"},
        {toolchain_option::include_file,    "-include"},
        {toolchain_option::include_sysfile, "-sys-include"},

        {toolchain_option::output_object,         "-o"},
        {toolchain_option::output_pch,            "-o"},
        {toolchain_option::output_debug,          ""},
        {toolchain_option::output_import_library, "-Wl,--out-implib,"},

        {toolchain_option::compile_only,  "-c"},
        {toolchain_option::compile_debug, "-g"},

        {toolchain_option::object_extension, ".o"},

        {toolchain_option::optimize_none,    "-O0"},
        {toolchain_option::optimize_normal,  "-O2"},
        {toolchain_option::optimize_size,    "-Os"},
        {toolchain_option::optimize_extreme, "-O3"},
        {toolchain_option::optimize_link,    "-flto"},
        {toolchain_option::optimize_extra,   "-fomit-frame-pointer -momit-leaf-frame-pointer"},

        {toolchain_option::dynamic_linker_c,       "gcc"},
        {toolchain_option::dynamic_linker_cxx,     "g++"},
        {toolchain_option::dynamic_linker_fortran, "gfortran"},

        {toolchain_option::static_linker,       "ar"},
        {toolchain_option::static_link_options, "rcsP "},

        {toolchain_option::link_debug,            ""},
        {toolchain_option::link_optimize,         "-flto"},
        {toolchain_option::link_strip,            "-s"},
        {toolchain_option::link_library,          "-l"},
        {toolchain_option::link_search_directory, "-L"},

        {toolchain_option::static_library_prefix,    "lib"},
        {toolchain_option::static_library_extension, ".a"},
        {toolchain_option::shared_library_prefix,    "lib"},
        {toolchain_option::import_library_extension, ".dll.a"},
      }
    },
    {toolchain::Microsoft,
      {
        {toolchain_option::compiler_c,      "cl"},
        {toolchain_option::compiler_cxx,    "cl"},
        {toolchain_option::compiler_nologo, "/NOLOGO"},

        {toolchain_option::define, "/D"},

        {toolchain_option::include_dir,     "/I"},
        {toolchain_option::include_file,    "/FI"},
        {toolchain_option::include_pch,     "/Fp"},
        {toolchain_option::include_sysfile, ""},

        {toolchain_option::output_object,         "/Fo"},
        {toolchain_option::output_pch,            "/Yc"},
        {toolchain_option::output_debug,          ""},
        {toolchain_option::output_import_library, "/implib"},

        {toolchain_option::compile_only,  "/c"},
        {toolchain_option::compile_debug, "/Zi"},

        {toolchain_option::object_extension, ".obj"},

        {toolchain_option::optimize_none,    "/Od"},
        {toolchain_option::optimize_normal,  "/O2"},
        {toolchain_option::optimize_size,    "/O1"},
        {toolchain_option::optimize_extreme, "/Ox"},
        {toolchain_option::optimize_link,    "/GL"},
        {toolchain_option::optimize_extra,   ""},

        {toolchain_option::dynamic_linker_c,       "link"},
        {toolchain_option::dynamic_linker_cxx,     "link"},

        {toolchain_option::static_linker,       "link"},
        {toolchain_option::static_link_options, "/LIB /OUT:"},

        {toolchain_option::link_debug,            "/DEBUG"},
        {toolchain_option::link_optimize,         "/LTCG /NOWIN98"},
        {toolchain_option::link_strip,            "/OPT:ICF"},
        {toolchain_option::link_library,          ""},
        {toolchain_option::link_search_directory, "/libpath:"},

        {toolchain_option::static_library_prefix,    ""},
        {toolchain_option::static_library_extension, ".lib"},
        {toolchain_option::shared_library_prefix,    ""},
        {toolchain_option::import_library_extension, ".dll.lib"},
      }
    },
    {toolchain::LLVM,
      {
        {toolchain_option::compiler_c,      "clang"},
        {toolchain_option::compiler_cxx,    "clang++"},
        {toolchain_option::compiler_nologo, ""},

        {toolchain_option::define, "-D"},

        {toolchain_option::include_dir,     "-I"},
        {toolchain_option::include_file,    "-include"},
        {toolchain_option::include_file,    "-include"},
        {toolchain_option::include_sysfile, "-sys-include"},

        {toolchain_option::output_object,         "-o"},
        {toolchain_option::output_pch,            "-o"},
        {toolchain_option::output_debug,          ""},
        {toolchain_option::output_import_library, "-Wl,--out-implib,"},

        {toolchain_option::compile_only,  "-c"},
        {toolchain_option::compile_debug, "-g"},

        {toolchain_option::object_extension, ".o"},

        {toolchain_option::optimize_none,    "-O0"},
        {toolchain_option::optimize_normal,  "-O2"},
        {toolchain_option::optimize_size,    "-Os"},
        {toolchain_option::optimize_extreme, "-O3"},
        {toolchain_option::optimize_link,    "-flto"},
        {toolchain_option::optimize_extra,   "-fomit-frame-pointer -momit-leaf-frame-pointer"},

        {toolchain_option::dynamic_linker_c,   "clang"},
        {toolchain_option::dynamic_linker_cxx, "clang++"},

        {toolchain_option::static_linker,       "ar"},
        {toolchain_option::static_link_options, "rcsP "},

        {toolchain_option::link_debug,            ""},
        {toolchain_option::link_optimize,         "-flto"},
        {toolchain_option::link_strip,            "-s"},
        {toolchain_option::link_library,          "-l"},
        {toolchain_option::link_search_directory, "-L"},

        {toolchain_option::static_library_prefix,    "lib"},
        {toolchain_option::static_library_extension, ".a"},
        {toolchain_option::shared_library_prefix,    "lib"},
        {toolchain_option::import_library_extension, ".dll.a"},
      }
    }
  };

const std::map<toolchain, std::map<file_type, language_option_map>> language_options =
  {
    {toolchain::GNU,
      {
        {file_type::source_c,
          {
            {language_option::compile_language, "-x c"},
            {language_option::std_c89,          "-std=c89"},
            {language_option::std_c90,          "-std=c90"},
            {language_option::std_c99,          "-std=c99"},
            {language_option::std_c11,          "-std=c11"},
            {language_option::std_gnu89,        "-std=gnu89"},
            {language_option::std_gnu90,        "-std=gnu90"},
            {language_option::std_gnu99,        "-std=gnu99"},
            {language_option::std_gnu11,        "-std=gnu11"},
          }
        },
        {file_type::source_cxx,
          {
            {language_option::compile_language, "-x c++"},
            {language_option::std_cxx98,        "-std=c++98"},
            {language_option::std_cxx03,        "-std=c++03"},
            {language_option::std_cxx11,        "-std=c++11"},
            {language_option::std_gnuxx98,      "-std=gnu++98"},
            {language_option::std_gnuxx03,      "-std=gnu++03"},
            {language_option::std_gnuxx11,      "-std=gnu++11"},
          }
        }
      }
    },
    {toolchain::Microsoft,
      {
        {file_type::source_c,
          {
            {language_option::compile_language, ""},
            {language_option::std_cxx98,        ""},
            {language_option::std_cxx03,        ""},
            {language_option::std_cxx11,        ""},
            {language_option::std_gnuxx98,      ""},
            {language_option::std_gnuxx03,      ""},
            {language_option::std_gnuxx11,      ""},
          }
        },
        {file_type::source_cxx,
          {
            {language_option::compile_language, ""},
            {language_option::std_cxx98,        ""},
            {language_option::std_cxx03,        ""},
            {language_option::std_cxx11,        ""},
            {language_option::std_gnuxx98,      ""},
            {language_option::std_gnuxx03,      ""},
            {language_option::std_gnuxx11,      ""},
          }
        }
      }
    },
    {toolchain::LLVM,
      {
        {file_type::source_c,
          {
            {language_option::compile_language, "-x c"},
            {language_option::std_c89,          "-std=c89"},
            {language_option::std_c90,          "-std=c90"},
            {language_option::std_c99,          "-std=c99"},
            {language_option::std_c11,          "-std=c11"},
            {language_option::std_gnu89,        "-std=gnu89"},
            {language_option::std_gnu90,        "-std=gnu90"},
            {language_option::std_gnu99,        "-std=gnu99"},
            {language_option::std_gnu11,        "-std=gnu11"},
          }
        },
        {file_type::source_cxx,
          {
            {language_option::compile_language, "-x c++"},
            {language_option::std_cxx98,        "-std=c++98"},
            {language_option::std_cxx03,        "-std=c++03"},
            {language_option::std_cxx11,        "-std=c++11"},
            {language_option::std_gnuxx98,      "-std=gnu++98"},
            {language_option::std_gnuxx03,      "-std=gnu++03"},
            {language_option::std_gnuxx11,      "-std=gnu++11"},
          }
        }
      }
    }
  };

extern const std::map<string, language_option> language_option_config_map =
  {
    {"c89", language_option::std_c89},
    {"c90", language_option::std_c90},
    {"c99", language_option::std_c99},
    {"c11", language_option::std_c11},

    {"GNU89", language_option::std_gnu89},
    {"GNU90", language_option::std_gnu90},
    {"GNU99", language_option::std_gnu99},
    {"GNU11", language_option::std_gnu11},

    {"C++89", language_option::std_cxx98},
    {"C++03", language_option::std_cxx03},
    {"C++11", language_option::std_cxx11},

    {"GNU++98", language_option::std_gnuxx98},
    {"GNU++03", language_option::std_gnuxx03},
    {"GNU++11", language_option::std_gnuxx11}
  };

const std::map<os, os_option_map> os_options =
  {
    {os::Windows,
      {
        {os_option::executable_extension,     ".exe"},
        {os_option::shared_library_extension, ".dll"},
      }
    },
    {os::Linux,
      {
        {os_option::executable_extension,     ""},
        {os_option::shared_library_extension, ".so"},
      }
    },
    {os::MacOSX,
      {
        {os_option::executable_extension,     ""},
        {os_option::shared_library_extension, ".dylib"},
      }
    }
  };

} // namespace lib

} // namespace ambrosia
