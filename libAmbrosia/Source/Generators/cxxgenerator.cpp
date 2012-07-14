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
 * Ambrosia/Generators/cxxgenerator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Generators/cxxgenerator.h"

// libAmbrosia includes
#include "Ambrosia/target.h"

libambrosia_namespace_begin

cxxgenerator::cxxgenerator(const target& target)
: cgenerator(target)
{   }

cxxgenerator::~cxxgenerator()
{   }

bool cxxgenerator::next_command(std::string& /*command*/)
{
  return false;
}


libambrosia_namespace_end
