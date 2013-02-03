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
 * Ambrosia/Error/soft_error.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/soft_error.h"

// C++ includes
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

soft_error::soft_error(const string& message)
: error(message)
{   }

soft_error::~soft_error()
{   }

void soft_error::output_message() const
{
  cerr << "Soft error:\n"
       << "\t" << message << "\n";
}

} // namespace lib

} // namespace ambrosia
