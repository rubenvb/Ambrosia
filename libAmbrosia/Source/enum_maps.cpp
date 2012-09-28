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
 * Ambrosia/enum_maps.cpp
 * Enum map definitions.
 *
 **/

// map declarations
#include "Ambrosia/enum_maps.h"

// libAmbrosia includes
#include "Ambrosia/boost_wrapper.h"

// C++ includes
#include <functional>
  using std::binary_function;
#include <map>
  using std::map;
#include <string>
  using std::string;

libambrosia_namespace_begin

const map<string, architecture> architecture_map =
  map_entries_begin
    entry_begin "x86",   architecture::x86 entry_end
    entry_begin "amd64", architecture::amd64 entry_end
  entries_end;
const map<architecture, string> architecture_map_inverse =
  map_entries_begin
    entry_begin architecture::x86,   "x86" entry_end
    entry_begin architecture::amd64, "amd64" entry_end
  entries_end;

const map<string, os> os_map =
  map_entries_begin
    entry_begin "windows", os::Windows entry_end
    entry_begin "linux",   os::Linux entry_end
    entry_begin "mac",     os::MacOSX entry_end
  entries_end;
const map<os, string> os_map_inverse =
  map_entries_begin
    entry_begin os::Windows, "windows" entry_end
    entry_begin os::Linux,   "linux" entry_end
    entry_begin os::MacOSX,  "mac" entry_end
  entries_end;

// libambrosia::toolchain has two maps: one for the ambrosia cross
// specification, another for true vendor name output. Both map to
// the same internal representation (i.e. the vendor name).
const map<string, toolchain> vendor_map =
  map_entries_begin
    entry_begin "GNU",       toolchain::GNU entry_end
    entry_begin "Microsoft", toolchain::Microsoft entry_end
    entry_begin "LLVM",      toolchain::LLVM entry_end
    entry_begin "Intel",     toolchain::Intel entry_end
  entries_end;
const map<toolchain, string> vendor_map_inverse =
  map_entries_begin
    entry_begin toolchain::GNU,       "GNU" entry_end
    entry_begin toolchain::Microsoft, "Microsoft" entry_end
    entry_begin toolchain::LLVM,      "LLVM" entry_end
    entry_begin toolchain::Intel,     "Intel" entry_end
  entries_end;

const map<string, toolchain> toolchain_map =
  map_entries_begin
    entry_begin "gcc",   toolchain::GNU entry_end
    entry_begin "msvc",  toolchain::Microsoft entry_end
    entry_begin "clang", toolchain::LLVM entry_end
    entry_begin"icc",   toolchain::Intel entry_end
  entries_end;
const map<toolchain, string> toolchain_map_inverse =
  map_entries_begin
    entry_begin toolchain::GNU,       "gcc" entry_end
    entry_begin toolchain::Microsoft, "msvc" entry_end
    entry_begin toolchain::LLVM,      "clang" entry_end
    entry_begin toolchain::Intel,     "icc" entry_end
  entries_end;

const map<string, environment> environment_map =
  map_entries_begin
    entry_begin "cmd",  environment::cmd entry_end
    entry_begin "Bash", environment::bash entry_end
  entries_end;
const map<environment, string> environment_map_inverse =
  map_entries_begin
    entry_begin environment::cmd,  "cmd" entry_end
    entry_begin environment::bash, "Bash" entry_end
  entries_end;

const map<string, target_type> target_type_map =
  map_entries_begin
    entry_begin "global",  target_type::global entry_end
    entry_begin "app",     target_type::app entry_end
    entry_begin "lib",     target_type::lib entry_end
    entry_begin "sub",     target_type::project entry_end
    entry_begin "install", target_type::install entry_end
    entry_begin "test",    target_type::test entry_end
  entries_end;
const map<target_type, string> target_type_map_inverse =
  map_entries_begin
    entry_begin target_type::global,  "global" entry_end
    entry_begin target_type::app,     "app" entry_end
    entry_begin target_type::lib,     "lib" entry_end
    entry_begin target_type::project, "sub" entry_end
    entry_begin target_type::install, "install" entry_end
    entry_begin target_type::test,    "test" entry_end
  entries_end;

const map<string, file_type> file_type_map =
  map_entries_begin
    entry_begin "SOURCES",         file_type::source entry_end
    entry_begin "SOURCES_C",       file_type::source_c entry_end
    entry_begin "SOURCES_CXX",     file_type::source_cxx entry_end
    entry_begin "SOURCES_JAVA",    file_type::source_java entry_end
    entry_begin "SOURCES_FORTRAN", file_type::source_fortran entry_end
    entry_begin "HEADERS",         file_type::header entry_end
    entry_begin "HEADERS_C",       file_type::header_c entry_end
    entry_begin "HEADERS_CXX",     file_type::header_cxx entry_end
    entry_begin "LIBS",            file_type::library entry_end
    entry_begin "RESOURCES",       file_type::resource entry_end
    entry_begin "QT_UI",           file_type::Qt_ui entry_end
    entry_begin "QT_MOC",          file_type::Qt_moc entry_end
    entry_begin "QT_RC",           file_type::Qt_rc entry_end
  entries_end;
const map<file_type, string> file_type_map_inverse =
  map_entries_begin
    entry_begin file_type::source,         "SOURCES" entry_end
    entry_begin file_type::source_c,       "SOURCES_C" entry_end
    entry_begin file_type::source_cxx,     "SOURCES_CXX" entry_end
    entry_begin file_type::source_java,    "SOURCES_JAVA" entry_end
    entry_begin file_type::source_fortran, "SOURCES_FORTRAN" entry_end
    entry_begin file_type::header,         "HEADERS" entry_end
    entry_begin file_type::header_c,       "HEADERS_C" entry_end
    entry_begin file_type::header_cxx,     "HEADER_CXX" entry_end
    entry_begin file_type::library,        "LIBS" entry_end
    entry_begin file_type::resource,       "RESOURCES" entry_end
    entry_begin file_type::Qt_ui,          "QT_UI" entry_end
    entry_begin file_type::Qt_moc,         "QT_MOC" entry_end
    entry_begin file_type::Qt_rc,          "Qt_RC" entry_end
  entries_end;
const map<string, file_type> directory_type_map =
  map_entries_begin
    entry_begin "SOURCE_DIRS",   file_type::source entry_end
    entry_begin "HEADER_DIRS",   file_type::header entry_end
    entry_begin "RESOURCE_DIRS", file_type::resource entry_end
    entry_begin "QT_UI_DIRS",    file_type::Qt_ui entry_end
    entry_begin "QT_MOC_DIRS",   file_type::Qt_moc entry_end
    entry_begin "QT_RC_DIRS",    file_type::Qt_rc entry_end
  entries_end;
const map<file_type, string> directory_type_map_inverse =
  map_entries_begin
    entry_begin file_type::source,   "SOURCE_DIRS" entry_end
    entry_begin file_type::header,   "HEADER_DIRS" entry_end
    entry_begin file_type::resource, "RESOURCE_DIRS" entry_end
    entry_begin file_type::Qt_ui,    "QT_UI_DIRS" entry_end
    entry_begin file_type::Qt_moc,   "QT_MOC_DIRS" entry_end
    entry_begin file_type::Qt_rc,    "QT_RC_DIRS" entry_end
  entries_end;

const map<string, conditional_operator> conditional_operator_map =
  map_entries_begin
    entry_begin ")", conditional_operator::right_parenthesis entry_end
    entry_begin "(", conditional_operator::left_parenthesis entry_end
    entry_begin "+", conditional_operator::and_op entry_end
    entry_begin "|", conditional_operator::or_op entry_end
    entry_begin "!", conditional_operator::not_op entry_end
  entries_end;
const map<conditional_operator, std::string> conditional_operator_map_inverse =
  map_entries_begin
    entry_begin conditional_operator::right_parenthesis, ")" entry_end
    entry_begin conditional_operator::left_parenthesis,  "(" entry_end
    entry_begin conditional_operator::and_op,           "+" entry_end
    entry_begin conditional_operator::or_op,             "|" entry_end
    entry_begin conditional_operator::not_op,            "!" entry_end
  entries_end;

libambrosia_namespace_end
