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
 * utility.c++
 * Utilities that may be useful in more than 1 place.
 *
 **/

// Function include
#include "Ambrosia/utility.h++"

// libAmbrosia includes
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/Error/internal_error.h++"
#include "Ambrosia/platform.h++"
#include "Ambrosia/Target/target.h++"

// C++ includes
#include <algorithm>
#include <ostream>
  using std::ostream;

namespace ambrosia
{
  namespace lib
  {
    ostream& operator<<(ostream& os, const string_set& strings)
    {
      if(strings.empty())
        os << "\t<empty list>\n";
      else
      {
        for(auto&& item : strings)
        {
          os << "\t" << item << "\n";
        }
      }
      return os;
    }
    ostream& operator<<(ostream& os, const std::set<file_type>& file_type_set)
    {
      if(file_type_set.empty())
        os << "\t<empty list>\n";
      else
      {
        for(auto&& type : file_type_set)
        {
          os << "\t" << file_type_map_inverse.at(type);
        }
      }
      return os;
    }
    ostream& operator<<(ostream& os, const platform::command& command)
    {
    #if _WIN32
      os << platform::convert_to_utf8(command.arguments);
    #else
      std::for_each(std::begin(command.array), std::end(command.array)-1,[](const string& argument) { os << " " << argument; });
    #endif
      return os;
    }

    ostream& operator<<(ostream& os, const command_vector& commands)
    {
      for(auto&& command : commands)
      {
        os << command << "\n";
      }
      return os;
    }

    ostream& operator<<(ostream& os, const dependency_map& dependencies)
    {
      if(dependencies.empty())
        os << "\t<empty list>\n";
      for(auto&& target_list : dependencies)
      {
        os << "\t" << target_type_map_inverse.at(target_list.first) << ":\n";
        for(auto& target : target_list.second)
        {
          if(target==nullptr)
            throw internal_error("Nullptr target.");
          os << "\t\t" << target->name << "\n";
        }
      }
      return os;
    }
  }
}
