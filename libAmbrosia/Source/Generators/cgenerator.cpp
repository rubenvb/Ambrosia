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
 * Ambrosia/Generators/cgenerator.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Generators/cgenerator.h"

// libAmbrosia includes
#include "Ambrosia/target.h"

// C++ includes
#include <sstream>
  using std::ostringstream;

libambrosia_namespace_begin

cgenerator::cgenerator(const file_type type,
                       const target& target)
: generator(type, target),
  current(target.source_files(m_type).begin())
{   }

cgenerator::~cgenerator()
{   }

bool cgenerator::next_command(std::string& command)
{

  ostringstream stream;
  stream << command;

  return false;
}

libambrosia_namespace_end
