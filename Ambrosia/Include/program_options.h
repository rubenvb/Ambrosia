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
 * program_options.h
 * Struct containing all internal commandline options.
 *
 **/

#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

ambrosia_namespace_begin

struct program_options
{
  program_options() : dump_commands(false), target_options()
  {   }
  bool dump_commands;
  map_string_set_string target_options;
};

ambrosia_namespace_end

#endif // AMBROSIA_OPTIONS_H
