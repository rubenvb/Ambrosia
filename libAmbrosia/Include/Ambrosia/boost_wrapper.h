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
 * Ambrosia/boost_wrapper.h
 * Wrapper macros for C++11 constructs which can be mimicked by Boost and macros.
 *
 **/

#ifndef AMBROSIA_BOOST_WRAPPER
#define AMBROSIA_BOOST_WRAPPER

#ifdef _MSC_VER // Intel and MSVC Compilers on Windows
#include <utility>
#include <boost/assign/list_of.hpp>
#define list_entries_begin boost::assign::list_of
#define map_entries_begin boost::assign::map_list_of
#define entries_end
#define entry_begin (
#define entry_end )
#else // GCC and Clang support C++11 brace init
#define list_entries_begin {
#define map_entries_begin {
#define entries_end }
#define entry_begin {
#define entry_end },
#endif

#endif
