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

#ifndef AMBROSIA_BUILD_ELEMENT_H
#define AMBROSIA_BUILD_ELEMENT_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/file.h"

libambrosia_namespace_begin

struct build_element
{
  file source_file;
  mutable file object_file;
  bool already_built;

  build_element(const file& source, const file& object = file())
  : source_file(source), object_file(object)
  {   }

  bool operator<(const build_element& rhs) const
  { return source_file.name < rhs.source_file.name; }
  bool operator==(const build_element& rhs) const
  { return source_file.name == rhs.source_file.name; }
};

libambrosia_namespace_end

// for unordered_* containers
namespace std
{
  template <>
  struct hash<libambrosia::build_element>
  {
    std::size_t operator()(const libambrosia::build_element& b) const
    { return hash<string>()(b.source_file.name); }
  };
}

#endif // AMBROSIA_BUILD_ELEMENT_H
