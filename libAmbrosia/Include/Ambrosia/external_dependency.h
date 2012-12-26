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
 * Ambrosia/external_dependency.h
 * Internal representation of the commandline input for external dependencies.
 *
 **/

#ifndef AMBROSIA_EXTERNAL_DEPENDENCY_H
#define AMBROSIA_EXTERNAL_DEPENDENCY_H

// Global include
#include "Ambrosia/global.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

// Function declaration
const std::string operator/(const std::string&,const std::string&);

struct external_dependency
{
  external_dependency() : install_prefix(), lib(), bin(), include() {};
  external_dependency(const std::string& install_prefix)
  : install_prefix(install_prefix), lib(install_prefix/"lib"), bin(install_prefix/"bin"), include(install_prefix/"include")
  {   }
  std::string install_prefix;
  std::string lib;
  std::string bin;
  std::string include;
};

libambrosia_namespace_end

#endif // AMBROSIA_EXTERNAL_DEPENDENCY_H
