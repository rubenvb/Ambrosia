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
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Generators/compile_and_link_generator.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/build_element.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/status.h"
#include "Ambrosia/Targets/project.h"
#include "Ambrosia/Targets/target.h"

// C++ includes
#include <sstream>
  using std::ostringstream;
#include <string>
  using std::string;

libambrosia_namespace_begin

/*compile_and_link_generator::compile_and_link_generator(const file_type type,
                                                       const build_element_set files,
                                                       const lib::configuration& configuration)
: generator(type, configuration)
{
  debug(debug::command_gen) << "compile_and_link_generator::compile_and_link_generator::Generator created for output in \""
                            << configuration.build_directory << "\"\n";
}

compile_and_link_generator::~compile_and_link_generator()
{   }

*/

libambrosia_namespace_end
