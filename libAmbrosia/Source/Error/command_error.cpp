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
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/command_error.h"

// C++ includes
#include <algorithm>
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

libambrosia_namespace_begin

command_error::command_error(const string& error_output,
                             const platform::command& failed_command)
: error(error_output),
  command()
{
  command.append(failed_command.program);
  std::for_each(std::begin(failed_command.arguments), std::end(failed_command.arguments),
  [this](const char* arg) { command.append(arg); });
}

void command_error::output_message() const
{
  throw 1;
  cerr << "Error: command returned an error.\n"
       << "Failed command: " << command << "\n"
       << "Error output:\n" << message << "\n";
}

libambrosia_namespace_end
