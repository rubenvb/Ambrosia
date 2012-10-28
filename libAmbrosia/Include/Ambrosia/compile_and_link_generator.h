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
 * Ambrosia/Generators/compile_and_link_generator.cpp
 * Command generator for C-style languages.
 *  Base class for C(, ObjC, ObjC++)
 *  Might be extended/improved for every compile+link language in the future.
 *
 **/

#ifndef AMBROSIA_GENERATORS_COMPILE_AND_LINK_GENERATOR_H
#define AMBROSIA_GENERATORS_COMPILE_AND_LINK_GENERATOR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/Generators/generator.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class target;

/*class compile_and_link_generator : public generator
{
public:
  compile_and_link_generator(const file_type type,
                             const lib::configuration& configuration);
  virtual ~compile_and_link_generator();

  virtual void generate_object_filenames();

  virtual const string_vector generate_parallel_commands();

  virtual const string_vector generate_final_commands();
};*/

libambrosia_namespace_end

#endif // AMBROSIA_GENERATORS_COMPILE_AND_LINK_GENERATOR_H
