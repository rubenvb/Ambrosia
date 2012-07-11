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
 * Ambrosia/Generators/cxxgenerator.h
 *
 *
 **/

#ifndef AMBROSIA_CXXGENERATOR_H
#define AMBROSIA_CXXGENERATOR_H

// libAmbrosia includes
#include "Ambrosia/Generators/cgenerator.h"

libambrosia_namespace_begin

// Forward declarations
class build_config;

class cxxgenerator : public cgenerator
{
public:
  cxxgenerator(const build_config& config);
  virtual ~cxxgenerator();

  bool next_command(std::string& command);
};

libambrosia_namespace_end

#endif // AMBROSIA_CXXGENERATOR_H
