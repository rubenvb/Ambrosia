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
 * Ambrosia/link_generator.h
 * Class responsible for generating link and "post-processing" commands.
 *
 **/

#ifndef LINK_GENERATOR_H
#define LINK_GENERATOR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

namespace ambrosia
{
namespace lib
{

// Forward declarations
class configuration;

/*class link_generator
{
public:
  link_generator(const class configuration& configuration,
                 const string_vector& libraries,
                 const std::map<file_type, build_element_set>& files);

  const command_vector generate_final_commands() const;

private:
  const configuration& configuration;
  string_vector libraries;
  const std::map<file_type, build_element_set>& files;
  target_type type;
};*/

} // namespace lib

} // namespace ambrosia

#endif // LINK_GENERATOR_H
