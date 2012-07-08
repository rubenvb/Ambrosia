/***
  * Ambrosia/debug.h
  * Class designed to be used as temporary output stream for debugging purposes.
  *
  * Author: Ruben Van Boxem
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
    commandline   = 1 << 0,
    algorithm     = 1 << 1,
    nectar        = 1 << 2,
    lexer         = 1 << 3,
    parser        = 1 << 4,
    nectar_parser = nectar | parser,
    conditional   = 1 << 5,
    target        = 1 << 6,
    files         = 1 << 7,
    platform      = 1 << 8,
    status        = 1 << 9,
    config        = 1 << 10,
    command_gen   = 1 << 11,
    initial       = 1 << 30,
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
