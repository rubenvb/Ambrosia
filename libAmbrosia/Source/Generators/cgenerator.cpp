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
#include "Ambrosia/debug.h"
#include "Ambrosia/target.h"

// C++ includes
#include <sstream>
  using std::ostringstream;
#include <string>
  using std::string;

libambrosia_namespace_begin

cgenerator::cgenerator(const file_type type,
                       const target& target)
: generator(type, target)
{   }

cgenerator::~cgenerator()
{   }

const string_vector cgenerator::generate_commands()
{
  string_vector commands;
  std::stringstream command;
  // compile commands
  for(auto it = m_target.source_files(m_type).begin(); it != m_target.source_files(m_type).end(); ++it)
  {

  }
  // link command
  commands.push_back(generate_link_command());

  return commands;
}

const string cgenerator::generate_link_command()
{
  return "";
}

libambrosia_namespace_end
