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
 * Ambrosia/Generators/cgenerator.cpp
 * Command generator for C-style languages.
 *  Base class for C(, ObjC, ObjC++)
 *
 **/

#ifndef CGENERATOR_H
#define CGENERATOR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Generators/generator.h"
#include "Ambrosia/typedefs.h"

libambrosia_namespace_begin

// Forward declarations
class build_config;

class cgenerator : public generator
{
public:
  cgenerator(const build_config& config);
  virtual ~cgenerator();

  virtual bool next_command(std::string& command);
};

libambrosia_namespace_end

#endif // CGENERATOR_H
