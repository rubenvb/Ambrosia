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
 * Ambrosia/Error/platform_error.cpp
 * Error class for platform API specific error handling.
 *
 **/

#ifndef AMBROSIA_ERROR_PLATFORM_ERROR_H
#define AMBROSIA_ERROR_PLATFORM_ERROR_H

// Global include
#include "Ambrosia/platform.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

namespace ambrosia
{
namespace lib
{

class platform_error : public error
{
public:
  platform_error();
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_ERROR_PLATFORM_ERROR_H
