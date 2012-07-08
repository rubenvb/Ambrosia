/**
  * Ambrosia/debug.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/debug.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/enum_maps.h"

// C++ includes
#include <algorithm>
#include <map>
  using std::map;
#include <set>
  using std::set;

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
// static member initialization
debug::type debug::s_level = // debug::initial
        static_cast<debug::type>(debug::commandline ^ debug::platform ^ debug::initial ^ debug::command_gen);

extern const map<std::string, debug::type> debug_map =
                { {"commandline",   debug::commandline},
                  {"algorithm",     debug::algorithm},
                  {"nectar",        debug::nectar},
                  {"lexer",         debug::lexer},
                  {"parser",        debug::parser},
                  {"NoT ReAcHABle", debug::nectar_parser},
                  {"conditional",   debug::conditional},
                  {"target",        debug::target},
                  {"files",         debug::files},
                  {"platform",      debug::platform},
                  {"status",        debug::status},
                  {"config",        debug::config},
                  {"command_gen",   debug::command_gen},
                  {"initial",       debug::initial},
                  {"all",           debug::always} };
extern const map<debug::type, std::string> debug_map_inverse =
                { {debug::commandline,   "commandline"},
                  {debug::algorithm,     "algorithm"},
                  {debug::nectar,        "nectar"},
                  {debug::lexer,         "lexer"},
                  {debug::parser,        "parser"},
                  {debug::nectar_parser, "nectar and parser"},
                  {debug::conditional,   "conditional"},
                  {debug::target,        "target"},
                  {debug::files,         "files"},
                  {debug::platform,      "platform"},
                  {debug::status,        "status"},
                  {debug::config,        "config"},
                  {debug::command_gen,   "command_gen"},
                  {debug::initial,       "initial"},
                  {debug::always,        "all"} };

debug::debug(const type debug_level)
: m_output(debug_level& s_level)
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
      std::for_each(strings.begin(), strings.end(), [strings](const std::string& item) { std::cerr << "\t" << item << "\n"; });
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
      std::for_each(type_list.begin(), type_list.end(), [type_list](const file_type type){ std::cerr << "\t" << map_value(file_type_map_inverse, type); });
  }
  return *this;
}

#endif // AMBROSIA_DEBUG


libambrosia_namespace_end
