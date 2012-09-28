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
#include "Ambrosia/Generators/generator_maps.h"

// C++ includes
#include <memory>
  using std::unique_ptr;

libambrosia_namespace_begin

generator::generator(const file_type type,
                     const target& target)
: m_type(type),
  m_target(target)
{  }

generator::~generator()
{   }

libambrosia_namespace_end
