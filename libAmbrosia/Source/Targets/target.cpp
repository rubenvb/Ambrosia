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
 * Ambrosia/Targets/target.cpp
 * Class implementation
 *
 **/

// Class include
#include "Ambrosia/Targets/target.h"

// C++ includes
#include <cstddef>
  using std::size_t;
#include <string>
  using std::string;

libambrosia_namespace_begin

target::target(const ::libambrosia::configuration& configuration,
               const string& name,
               const dependency_set& dependencies)
: name(name),
  configuration(configuration),
  dependencies(dependencies)
{   }

void target::add_source_file(const file_type /*type*/,
                             const std::string& /*filename*/,
                             file_cache& /*file_cache*/,
                             const std::string& /*nectar_file*/,
                             const size_t /*line_number*/)
{

}
bool target::add_source_directory(const file_type /*type*/,
                                  const std::string& /*directory*/,
                                  file_cache& /*file_cache*/,
                                  const std::string& /*nectar_file*/,
                                  const size_t /*line_number*/)
{
  return false;
}
bool target::add_library(const string& /*library*/,
                         const string& /*nectar_file*/,
                         const size_t /*line_number*/)
{
  return false;
}


libambrosia_namespace_end
