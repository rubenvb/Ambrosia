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
 * Error/nectar_error.c++
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/nectar_error.h++"

// linAmbrosia includes
#include "Ambrosia/algorithm.h++"
#include "Ambrosia/typedefs.h++"

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

nectar_error::nectar_error(const string& message,
                           const string& filename,
                           const size_t line_number,
                           const string_vector& list)
: error(message, list),
  m_filename(filename),
  m_line_number(line_number)
{   }

 void nectar_error::output_message() const
{
   cerr << "Error in: " << m_filename << "\n"
   << "line: " << to_string(m_line_number) << "\n";
   error::output_message();
}

 } // namespace lib

 } // namespace ambrosia
