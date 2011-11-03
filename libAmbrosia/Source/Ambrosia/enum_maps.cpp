/**
  * Ambrosia/enum_maps.cpp
  * Enum map definitions.
  *
  * Author: Ruben Van Boxem
  *
  **/

// map declarations
#include "Ambrosia/enum_maps.h"

// C++ includes
/* <functional> */
    using std::binary_function;
/* <map> */
    using std::map;
/* <string> */
    using std::string;

libambrosia_namespace_begin

const map<string, architecture> architecture_map =
         { {"x86",   architecture::x86},
           {"amd64", architecture::amd64} };
extern const map<architecture, string> architecture_map_inverse =
        { {architecture::x86,   "x86"},
          {architecture::amd64, "amd64"} };

const map<string, os> os_map =
         { {"windows", os::Windows},
           {"linux",   os::Linux},
           {"mac",     os::MacOSX} };
const map<os, string> os_map_inverse =
         { {os::Windows, "windows"},
           {os::Linux,   "linux"},
           {os::MacOSX,  "mac"} };

// libambrosia::toolchain has two maps: one for the ambrosia cross
// specification, another for true vendor name output. Both map to
// the same internal representation (i.e. the vendor name).
const map<string, toolchain> vendor_map =
         { {"GNU",       toolchain::GNU},
           {"Microsoft", toolchain::Microsoft},
           {"LLVM",      toolchain::LLVM},
           {"Intel",     toolchain::Intel} };
const map<toolchain, string> vendor_map_inverse =
        { {toolchain::GNU,       "GNU"},
          {toolchain::Microsoft, "Microsoft"},
          {toolchain::LLVM,      "LLVM"},
          {toolchain::Intel,     "Intel"} };

const map<string, toolchain> toolchain_map =
         { {"gcc",   toolchain::GNU},
           {"msvc",  toolchain::Microsoft},
           {"clang", toolchain::LLVM},
           {"icc",   toolchain::Intel} };
const map<toolchain, string> toolchain_map_inverse =
         { {toolchain::GNU,       "gcc"},
           {toolchain::Microsoft, "msvc"},
           {toolchain::LLVM,      "clang"},
           {toolchain::Intel,     "icc"} };

const map<string, environment> environment_map =
         { {"cmd",  environment::cmd},
           {"Bash", environment::bash} };
const map<environment, string> environment_map_inverse =
         { {environment::cmd,  "cmd"},
           {environment::bash, "Bash"} };

const map<string, target_type> target_type_map =
         { {"global",  target_type::global},
           {"app",     target_type::app},
           {"lib",     target_type::lib},
           {"sub",     target_type::sub},
           {"install", target_type::install},
           {"test",    target_type::test} };
const map<target_type, string> target_type_map_inverse =
         { {target_type::global,  "global"},
           {target_type::app,     "app"},
           {target_type::lib,     "lib"},
           {target_type::sub,     "sub"},
           {target_type::install, "install"},
           {target_type::test,    "test"} };

const map<string, file_type> file_type_map =
         { {"SOURCES",         file_type::source},
           {"SOURCES_C",       file_type::source_c},
           {"SOURCES_CXX",     file_type::source_cxx},
           {"SOURCES_JAVA",    file_type::source_java},
           {"SOURCES_FORTRAN", file_type::source_fortran},
           {"HEADERS",         file_type::header},
           {"RESOURCES",       file_type::resource},
           {"QT_UI",           file_type::Qt_ui},
           {"QT_MOC",          file_type::Qt_moc},
           {"QT_RC",           file_type::Qt_rc} };
const map<file_type, string> file_type_map_inverse =
         { {file_type::source,         "SOURCES"},
           {file_type::source_c,       "SOURCES_C"},
           {file_type::source_cxx,     "SOURCES_CXX"},
           {file_type::source_java,    "SOURCES_JAVA"},
           {file_type::source_fortran, "SOURCES_FORTRAN"},
           {file_type::header,         "HEADERS"},
           {file_type::resource,       "RESOURCES"},
           {file_type::Qt_ui,          "QT_UI"},
           {file_type::Qt_moc,         "QT_MOC"},
           {file_type::Qt_rc,          "Qt_RC"} };
const map<string, file_type> directory_type_map =
         { {"SOURCE_DIRS",   file_type::source},
           {"HEADER_DIRS",   file_type::header},
           {"RESOURCE_DIRS", file_type::resource},
           {"QT_UI_DIRS",    file_type::Qt_ui},
           {"QT_MOC_DIRS",   file_type::Qt_moc},
           {"QT_RC_DIRS",    file_type::Qt_rc} };
const map<file_type, string> directory_type_map_inverse =
         { {file_type::source,   "SOURCE_DIRS"},
           {file_type::header,   "HEADER_DIRS"},
           {file_type::resource, "RESOURCE_DIRS"},
           {file_type::Qt_ui,    "QT_UI_DIRS"},
           {file_type::Qt_moc,   "QT_MOC_DIRS"},
           {file_type::Qt_rc,    "QT_RC_DIRS"} };

const map<string, conditional_operator> conditional_operator_map =
         { {")", conditional_operator::right_parenthesis},
           {"(", conditional_operator::left_parenthesis},
           {"+", conditional_operator::and_op},
           {"|", conditional_operator::or_op},
           {"!", conditional_operator::not_op} };
const map<conditional_operator, std::string> conditional_operator_map_inverse =
         { {conditional_operator::right_parenthesis, ")"},
           {conditional_operator::left_parenthesis,  "("},
           {conditional_operator::and_op,           "+"},
           {conditional_operator::or_op,             "|"},
           {conditional_operator::not_op,            "!"} };

libambrosia_namespace_end
