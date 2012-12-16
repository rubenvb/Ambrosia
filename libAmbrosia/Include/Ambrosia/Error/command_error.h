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
 * Ambrosia/Error/command_error.h
 * Error displaying the failed command
 *
 **/

#ifndef AMBROSIA_COMMAND_ERROR_H
#define AMBROSIA_COMMAND_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"
#include "Ambrosia/platform.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class command_error : public error
{
public:
  command_error(const std::string& message,
                const platform::command& failed_command);

  void output_message() const;

private:
  std::string command;
};

libambrosia_namespace_end

#endif // AMBROSIA_COMMAND_ERROR_H
