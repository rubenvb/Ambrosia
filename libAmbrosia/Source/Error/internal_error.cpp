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
 * Ambrosia/Error/error.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/internal_error.h"

// C++ includes
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

internal_error::internal_error(const string& message)
: error(message)
{   }

void internal_error::output_message() const
{
  cerr << "This is an Ambrosia bug. A nasty, smelly bug. Please send in your project file and "
          "the output below to the Ambrosia project so this can be resolved as soon as possible.\n";
  error::output_message();
}

} // namespace lib

} // namespace ambrosia
