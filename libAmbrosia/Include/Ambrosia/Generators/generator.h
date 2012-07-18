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

  virtual const string_vector generate_commands() = 0;

protected:
  const file_type m_type;
  const target& m_target;
};

std::unique_ptr<generator> get_generator(const file_type type,
                                         const target& target);

libambrosia_namespace_end

#endif // GENERATORSGENERATOR_H
