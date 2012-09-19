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
 * Ambrosia/global.h
 * Global header included by all other Ambrosia library headers.
 *
 **/

#ifndef AMBROSIA_GLOBAL_H
#define AMBROSIA_GLOBAL_H

// namespace definition
#define ambrosia_namespace_begin namespace ambrosia {
#define ambrosia_namespace_end }
#define libambrosia_namespace_begin namespace ambrosia { namespace lib {
#define libambrosia_namespace_end } }

// deleted function workarounds
#ifdef _MSC_VER
#define DELETED_FUNCTION
#else
#define DELETED_FUNCTION = delete
#endif

libambrosia_namespace_begin

// Versioning
const int version_major = 0;
const int version_minor = 0;
const int version_bugfix = 1;

libambrosia_namespace_end

// namespace shortcut
namespace libambrosia = ambrosia::lib;

// Windows DLL magic
#if defined(AMBROSIA_DLL)
# if defined(AMBROSIA_BUILD_DLL)
#  define DLL_EXPORT  __declspec(dllexport)
# else // AMBROSIA_BUILD_DLL
#  define DLL_EXPORT  __declspec(dllimport)
# endif // AMBROSIA_BUILD_DLL
#else // AMBROSIA_DLL
# define DLL_EXPORT
#endif // AMBROSIA_DLL

#endif // AMBROSIA_GLOBAL_H
