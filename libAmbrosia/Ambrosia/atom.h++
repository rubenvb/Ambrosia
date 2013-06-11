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
 * Ambrosia/atom.h++
 * Basic building block.
 *
 **/

#ifndef AMBROSIA_ATOM_H
#define AMBROSIA_ATOM_H

// Global include
#include "Ambrosia/global.h++"

namespace ambrosia
{
  namespace lib
  {
    class atom
    {
    public:
      atom(const atom* const parent = nullptr);

    private:
      const atom* const parent; // project where this atom was defined in the project file
      std::vector<std::unique_ptr<atom>> sub_atoms; //
      std::vector<atom*> dependencies;
    };
  }
}

#endif
