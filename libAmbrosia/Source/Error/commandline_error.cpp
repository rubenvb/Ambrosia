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
 * Ambrosia/Error/commandline_error.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/commandline_error.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"

// C++ includes
#include <cstddef>
  using std::size_t;
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

commandline_error::commandline_error(const string& message,
                                     const size_t argument_number)
: error(message),
  m_argument_number(argument_number)
{   }

void commandline_error::output_message() const
{
  cerr << "Commandline error: argument " << to_string(m_argument_number) << ":\n";
  error::output_message();
}

} // namespace lib

} // namespace ambrosia
