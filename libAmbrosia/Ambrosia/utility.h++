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
 * Ambrosia/utility.h++
 * Utilities that may be useful in more than 1 place.
 *
 **/

#ifndef AMBROSIA_UTILITY_H
#define AMBROSIA_UTILITY_H

// Global include
#include "Ambrosia/global.h++"

// libAmbrosia includes
#include "typedefs.h++"

namespace ambrosia
{
  namespace lib
  {
    // tricks and fancies
    using swallow = int[]; // used in EXPAND to expand an expression using variadic args

    // output overloads
    std::ostream& operator<<(std::ostream& os, const string_set& strings);
    std::ostream& operator<<(std::ostream& os, const std::set<file_type>& file_types);
    std::ostream& operator<<(std::ostream& os, const platform::command& command);
    std::ostream& operator<<(std::ostream& os, const command_vector& commands);
    //std::ostream& operator<<(std::ostream& os, const external_dependency_set& dependencies);
    std::ostream& operator<<(std::ostream& os, const dependency_map& d);
  }
}

// execute expr over all variadic template arguments
#define EXPAND(expr) ambrosia::lib::swallow{0, ((expr),0)...}

#endif
