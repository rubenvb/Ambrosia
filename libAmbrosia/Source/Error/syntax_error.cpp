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
 * Ambrosia/Error/syntax_error.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/syntax_error.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

libambrosia_namespace_begin

syntax_error::syntax_error(const string& message,
                           const string& filename,
                           const size_t line_number,
                           const string_vector& list)
: nectar_error(message, filename, line_number, list)
{   }

void syntax_error::output_message() const
{
  cerr << "Syntax error:\n";
  nectar_error::output_message();
}

libambrosia_namespace_end
