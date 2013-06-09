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
 * debug.c++
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/debug.h++"

#ifdef AMBROSIA_DEBUG

// libAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/Target/external.h++"
#include "Ambrosia/enum_maps.h++"

// C++ includes
#include <algorithm>
#include <iostream>
  using std::cerr;
#include <map>
  using std::map;
#include <set>
  using std::set;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

// static member initialization
debug::type debug::level = debug::always;

const map<std::string, debug::type> debug_map =
  {
    {"commandline",   debug::commandline},
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
    {"command_exec",  debug::command_exec},
    {"dependencies",  debug::dependencies},
    {"all",           debug::always},
  };
const map<debug::type, std::string> debug_map_inverse =
  {
    {debug::commandline,   "commandline"},
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
    {debug::command_exec,  "command_exec"},
    {debug::dependencies,  "dependencies"},
    {debug::always,        "all"},
  };
/*
debug::debug(const type debug_level)
: output(0 != (debug_level & level)) // MSVC C4800 without "0 !="
#else // AMBROSIA_DEBUG
debug::debug(const type)
#endif // AMRBOSIA_DEBUG
{   }

#ifdef AMBROSIA_DEBUG
template<>
debug& debug::operator<<(const string_set& strings)
{
  if(output)
  {
    if(strings.empty())
      std::cerr << "\t<empty list>\n";
    else
    {
      for(auto&& item : strings)
      {
        cerr << "\t" << item << "\n";
      }
    }
  }
  return *this;
}
template<>
debug& debug::operator<<(const set<file_type>& type_list)
{
  if(output)
  {
    if(type_list.empty())
      std::cerr << "\t<empty list>\n";
    else
    {
      for(auto&& type : type_list)
      {
        cerr << "\t" << file_type_map_inverse.at(type);
      }
    }
  }
  return *this;
}
template<>
debug& debug::operator<<(const platform::command& command)
{
  if(output)
#if _WIN32
    std::cerr << platform::convert_to_utf8(command.arguments);
#else
    std::for_each(std::begin(command.array), std::end(command.array)-1,[](const string& argument) { std::cerr << " " << argument; });
#endif
  return *this;
}
template<>
debug& debug::operator<<(const command_vector& commands)
{
  for(auto&& command : commands)
  {
    this->operator<<(command) << "\n";
  }
  return *this;
}

template<>
debug& debug::operator<<(const external_dependency_set& dependencies)
{
  if(output)
  {
    if(dependencies.empty())
      std::cerr << "\t<empty list>\n";
    for(auto&& dependency : dependencies)
    {
      std::cerr << "\t" << dependency.name << "\n";
    }
  }
  return *this;
}
template<>
debug& debug::operator<<(const dependency_map& dependencies)
{
  if(output)
  {
    if(dependencies.empty())
      std::cerr << "\t<empty list>\n";
    for(auto&& target_list : dependencies)
    {
      std::cerr << "\t" << target_type_map_inverse.at(target_list.first) << ":\n";
      for(auto& target : target_list.second)
      {
        if(target==nullptr)
          throw internal_error("Nullptr target.");
        std::cerr << "\t\t" << target->name << "\n";
      }
    }
  }
  return *this;
}
*/
} // namespace lib

} // namespace ambrosia

#endif
