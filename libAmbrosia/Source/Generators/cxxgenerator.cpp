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

// C++ includes
#include <string>
  using std::string;

libambrosia_namespace_begin

cxxgenerator::cxxgenerator(const file_type type,
                           const target& target)
: cgenerator(type, target)
{   }

cxxgenerator::~cxxgenerator()
{   }

const string_vector cxxgenerator::generate_commands()
{
  string_vector commands;

  return commands;
}

const string cxxgenerator::generate_link_command()
{
  return "";
}

libambrosia_namespace_end
