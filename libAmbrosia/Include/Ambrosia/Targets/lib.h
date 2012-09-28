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
 * Ambrosia/Targets/lib.h
 * Library target for static and dynamic link libraries.
 *
 **/

#ifndef AMBROSIA_TARGETS_LIB_H
#define AMBROSIA_TARGETS_LIB_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Targets/target.h"

libambrosia_namespace_begin

class lib : public target
{
public:
  lib(const std::string& name,
      const build_config& configuration,
      const dependency_set& dependencies);
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_LIB_H
