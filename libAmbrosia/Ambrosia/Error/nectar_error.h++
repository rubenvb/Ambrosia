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
 * Ambrosia/Error/nectar_error.h++
 * Error related to a *.nectar.txt file.
 *
 **/

#ifndef AMBROSIA_NECTAR_ERROR_H
#define AMBROSIA_NECTAR_ERROR_H

// Global include
#include "Ambrosia/global.h++"

// libAmbrosia includes
#include "Ambrosia/Error/error.h++"

// C++ includes
#include <string>

namespace ambrosia
{
namespace lib
{

class nectar_error : public error
{
public:
  nectar_error(const std::string& message,
               const std::string& filename,
               const std::size_t line_number,
               const string_vector& list = {});

  virtual void output_message() const;

protected:
  const std::string m_filename;
  const std::size_t m_line_number;
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_NECTAR_ERROR_H
