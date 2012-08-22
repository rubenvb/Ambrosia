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
 * Ambrosia/build_element.h
 * Class representing a source file and its compiled output file.
 *
 **/

#ifndef FILE_H
#define FILE_H

// Global include
#include "global.h"

// C++ includes
#include <initializer_list>
#include <string>

libambrosia_namespace_begin

struct file
{
  std::string name;
  time_t time_modified;

  bool operator<(const file& rhs) const
  { return name < rhs.name; }
  bool operator==(const file& rhs) const
  { return name == rhs.name; }
};

libambrosia_namespace_end

namespace std
{
  template <>
  struct hash<libambrosia::file>
  {
    size_t operator()(const libambrosia::file& f) const
    { return hash<string>()(f.name); }
  };
}

#endif // FILE_H
