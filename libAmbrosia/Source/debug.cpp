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
 * Ambrosia/debug.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/debug.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/boost_wrapper.h"
#include "Ambrosia/enum_maps.h"

// C++ includes
#include <algorithm>
#include <map>
  using std::map;
#include <set>
  using std::set;
#include <string>
  using std::string;

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
// static member initialization
debug::type debug::s_level = debug::initial;

const map<std::string, debug::type> debug_map =
  map_entries_begin
    entry_begin "commandline",   debug::commandline entry_end
    entry_begin "algorithm",     debug::algorithm entry_end
    entry_begin "nectar",        debug::nectar entry_end
    entry_begin "lexer",         debug::lexer entry_end
    entry_begin "parser",        debug::parser entry_end
    entry_begin "NoT ReAcHABle", debug::nectar_parser entry_end
    entry_begin "conditional",   debug::conditional entry_end
    entry_begin "target",        debug::target entry_end
    entry_begin "files",         debug::files entry_end
    entry_begin "platform",      debug::platform entry_end
    entry_begin "status",        debug::status entry_end
    entry_begin "config",        debug::config entry_end
    entry_begin "command_gen",   debug::command_gen entry_end
    entry_begin "command_exec",  debug::command_exec entry_end
    entry_begin "dependencies",  debug::dependencies entry_end
    entry_begin "initial",       debug::initial entry_end
    entry_begin "all",           debug::always entry_end
  entries_end;
const map<debug::type, std::string> debug_map_inverse =
  map_entries_begin
    entry_begin debug::commandline,   "commandline" entry_end
    entry_begin debug::algorithm,     "algorithm" entry_end
    entry_begin debug::nectar,        "nectar" entry_end
    entry_begin debug::lexer,         "lexer" entry_end
    entry_begin debug::parser,        "parser" entry_end
    entry_begin debug::nectar_parser, "nectar and parser" entry_end
    entry_begin debug::conditional,   "conditional" entry_end
    entry_begin debug::target,        "target" entry_end
    entry_begin debug::files,         "files" entry_end
    entry_begin debug::platform,      "platform" entry_end
    entry_begin debug::status,        "status" entry_end
    entry_begin debug::config,        "config" entry_end
    entry_begin debug::command_gen,   "command_gen" entry_end
    entry_begin debug::command_exec,  "command_exec" entry_end
    entry_begin debug::dependencies,  "dependencies" entry_end
    entry_begin debug::initial,       "initial" entry_end
    entry_begin debug::always,        "all" entry_end
  entries_end;

debug::debug(const type debug_level)
: m_output(0 != (debug_level & s_level)) // MSVC C4800 without "0 !="
#else // AMBROSIA_DEBUG
debug::debug(const type)
#endif // AMRBOSIA_DEBUG
{   }

#ifdef AMBROSIA_DEBUG
template<>
debug& debug::operator<<(const string_set& strings)
{
  if(m_output)
  {
    if(strings.empty())
      std::cerr << "\t<empty list>\n";
    else
      std::for_each(std::begin(strings), std::end(strings), [strings](const std::string& item) { std::cerr << "\t" << item << "\n"; });
  }
  return *this;
}
template<>
debug& debug::operator<<(const set<file_type>& type_list)
{
  if(m_output)
  {
    if(type_list.empty())
      std::cerr << "\t<empty list>\n";
    else
      std::for_each(std::begin(type_list), std::end(type_list), [type_list](const file_type type){ std::cerr << "\t" << file_type_map_inverse.at(type); });
  }
  return *this;
}
template<>
debug& debug::operator<<(const platform::command& command)
{
  if(m_output)
#if _WIN32
    std::cerr << platform::convert_to_utf8(command.arguments);
#else
  {
    std::cerr << command.program;
    std::for_each(std::begin(command.argument_storage), std::end(command.argument_storage),[](const string& s) { std::cerr << " " << s; });
  }
#endif
  return *this;
}

#endif // AMBROSIA_DEBUG

libambrosia_namespace_end
