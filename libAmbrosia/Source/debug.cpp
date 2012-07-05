/**
  * Ambrosia/debug.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/debug.h"

// C++ includes
#include <algorithm>
#include <map>
    using std::map;

libambrosia_namespace_begin

#ifdef AMBROSIA_DEBUG
// static member initialization
debug::type debug::s_level = // debug::initial
        static_cast<debug::type>(debug::commandline ^ debug::platform ^ debug::initial);

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
                  {debug::initial,       "initial"},
                  {debug::always,        "all"} };

debug::debug( const type debug_level )
    :   m_output( debug_level& s_level )
#else // AMBROSIA_DEBUG
debug::debug( const type )
#endif // AMRBOSIA_DEBUG
{}

// specialization for string_set
template<>
#ifdef AMBROSIA_DEBUG
debug& debug::operator<<( const string_set& strings )
{
    if( m_output )
    {
        if( strings.empty() )
            std::cerr << "\t<empty list>\n";
        else
            std::for_each( strings.begin(), strings.end(),
                           [strings](const std::string& item)
                           { std::cerr << "\t" << item << "\n"; } );
    }
#else // AMBROSIA_DEBUG
debug& debug::operator<<( const string_set& )
{
#endif // AMBROSIA_DEBUG
    return *this;
}

libambrosia_namespace_end
