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
 * Ambrosia/debug.h
 * Class designed to be used as temporary output stream for debugging purposes.
 *
 **/

#ifndef AMBROSIA_DEBUG_H
#define AMBROSIA_DEBUG_H

// Ambrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/global.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iostream>

libambrosia_namespace_begin

class debug
{
public:
  enum type : uint32_t
  {
    commandline   = 1,
    algorithm     = 2,
    nectar        = 4,
    lexer         = 8,
    parser        = 16,
    nectar_parser = nectar | parser,
    conditional   = 32,
    target        = 64,
    files         = 128,
    platform      = 256,
    status        = 512,
    config        = 1024,
    command_gen   = 2048,
    command_exec  = 4096,
    initial       = 8192,
    always        = 0xffffffff
    // ...
  };

  typedef std::ostream& (*stream_function)(std::ostream&);

#ifdef AMBROSIA_DEBUG
  static type s_level;
  debug(const type debug_level = s_level);
#else // AMBROSIA_DEBUG
  debug(const type);
#endif // AMBROSIA_DEBUG
  template<typename T>
#ifdef AMBROSIA_DEBUG
  debug& operator<<(const T& output)
  {
    if(m_output)
      std::cerr << output;
    if(!std::cerr)
      throw internal_error("std::cerr was left in an invalid state.");
#else // AMBROSIA_DEBUG
  debug& operator<<( const T& )
  {
#endif // AMBROSIA_DEBUG
    return *this;
  }
  // for std::endl and other manipulators
#ifdef AMBROSIA_DEBUG
  debug& operator<<(stream_function func)
  {
    if(m_output)
      func(std::cerr);
    if(!std::cerr)
      throw internal_error("std::cerr was left in an invalid state.");
#else // AMBROSIA_DEBUG
  debug& operator<<(stream_function)
  {
#endif // AMBROSIA_DEBUG
    return *this;
  }

private:
#ifdef AMBROSIA_DEBUG
  const bool m_output;
#endif // AMBROSIA_DEBUG
};

#ifdef AMBROSIA_DEBUG
extern const std::map<std::string, debug::type> debug_map;
extern const std::map<debug::type, std::string> debug_map_inverse;

template<>
debug& debug::operator<<(const string_set&);
template<>
debug& debug::operator<<(const std::set<file_type>&);
#endif // AMBROSIA_DEBUG

libambrosia_namespace_end

#endif // AMBROSIA_DEBUG_H
