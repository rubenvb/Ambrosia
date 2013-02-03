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
 * Ambrosia/Error/syntax_error.h
 * A syntax error.
 *
 **/

#ifndef AMBROSIA_ERROR_SYNTAX_ERROR_H
#define AMBROSIA_ERROR_SYNTAX_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/nectar_error.h"

// C++ includes
#include <string>

namespace ambrosia
{
namespace lib
{

class syntax_error : public nectar_error
{
public:
  syntax_error(const std::string& message,
               const std::string& filename,
               const std::size_t line_number,
               const string_vector& list = string_vector());

  void output_message() const;
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_ERROR_SYNTAX_ERROR_H
