/**
 *
 * Project Ambrosia: Ambrosia commandline build tool
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
 * help_and_version_output.c++
 * Function implementation.
 *
 **/

// Function include
#include "help_and_version_output.h++"

// libAmbrosia includes
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/platform.h++"

// Ambrosia includes
#include "output.h++"

namespace ambrosia
{

void print_help_information()
{
  output() << "TODO: help output.\n";
}
void print_version_information()
{
  // Welcome message
  output() << "Welcome to Ambrosia, version " << app::version_major << "." << app::version_minor << "." << app::version_bugfix
           << ", using libAmbrosia version " << lib::version_major << "." << lib::version_minor << "." << lib::version_bugfix << ".\n"
           << "This version of Ambrosia was built for " << lib::os_map_inverse.at(lib::platform::build_os) << " " << lib::architecture_map_inverse.at(lib::platform::build_architecture)
           << " with the " << lib::vendor_map_inverse.at(lib::platform::ambrosia_toolchain) << " toolchain.\n\n";
}

} // namespace ambrosia
