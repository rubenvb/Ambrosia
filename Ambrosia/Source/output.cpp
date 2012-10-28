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
 * output.cpp
 * Class implementation
 *
 **/

// Class include
#include "output.h"

// C++ includes
#include <iostream>
  using std::cout;
  using std::ostream;

ambrosia_namespace_begin

// static member initialization
std::ostream& output::output_stream = std::cout;

ambrosia_namespace_end
