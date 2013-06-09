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
 * Ambrosia/build_element.h++
 * Class representing a source file and its compiled output file.
 *
 **/

#ifndef AMBROSIA_FILE_H
#define AMBROSIA_FILE_H

// Global include
#include "global.h++"

// C++ includes
#include <functional>
#include <string>

namespace ambrosia
{
namespace lib
{

struct file
{
  file(const std::string& name = {},
       const time_t time_modified = 0)
  : name(name), time_modified(time_modified)
  {}
  std::string name;
  time_t time_modified;

  bool operator<(const file& rhs) const
  { return name < rhs.name; }
  bool operator==(const file& rhs) const
  { return name == rhs.name; }
};

} // namespace lib

} // namespace ambrosia

namespace std
{
  template <>
  struct hash<libambrosia::file>
  {
    std::size_t operator()(const libambrosia::file& f) const
    { return hash<string>()(f.name); }
  };
}

#endif // AMBROSIA_FILE_H
