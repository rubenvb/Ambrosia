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
 * Ambrosia/Generators/generator.h
 * Abstract base class for command generation classes.
 *
 **/

#ifndef GENERATORSGENERATOR_H
#define GENERATORSGENERATOR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/enums.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <memory>
#include <string>

libambrosia_namespace_begin

// Forward declarations
class target;

class generator
{
public:
  generator(const file_type type,
            const target& target);
  virtual ~generator();

  // First, parallel executable commands, that are not influenced by eg missing link libraries from other targets
  // Example: the C/C++ compile commands that produce object files that will be linked together in the final step
  //TODO: what about generated headers? --> First make it possible to "generate headers"!!
  virtual const string_vector generate_parallel_commands() = 0;
  // Final (link) command that produces the target's output file(s). Cannot be called before the parallel commands have all  finished
  virtual const string_vector generate_final_commands() = 0;

protected:
  generator_map m_generator_map;
  const file_type m_type;
  const target& m_target;
};

libambrosia_namespace_end

#endif // GENERATORSGENERATOR_H
