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
 * global.h
 * Global header included in all Ambrosia Application headers.
 *
 **/

#ifndef GLOBAL_H
#define GLOBAL_H

// libAmbrosia global include
#include "Ambrosia/global.h"

ambrosia_namespace_begin

namespace app
{
  // Versioning
  const int version_major = 0;
  const int version_minor = 0;
  const int version_bugfix = 1;
}

ambrosia_namespace_end

#endif // GLOBAL_H
