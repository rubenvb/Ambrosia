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
 * Ambrosia/Generators/generator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Generators/generator.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/Generators/cgenerator.h"
#include "Ambrosia/Generators/cxxgenerator.h"

// C++ includes
#include <memory>
  using std::unique_ptr;

libambrosia_namespace_begin

generator::generator(const build_config& /*config*/)
{  }

unique_ptr<generator> get_generator(const file_type type, const build_config& config)
{
  switch(type)
  {
    case file_type::source_c:
      return unique_ptr<generator>(new cgenerator(config));
    case file_type::source_cxx:
      return unique_ptr<generator>(new cxxgenerator(config));
    default:
      throw internal_error("invalid command generator requested: " + map_value(file_type_map_inverse, type) + ".");
  }
}


libambrosia_namespace_end
