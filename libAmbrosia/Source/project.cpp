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
 * Ambrosia/project.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/project.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/build_element.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/Error/soft_error.h"
#include "Ambrosia/Generators/compile_and_link_generator.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
#include <iostream>
  using std::cout;
#include <memory>
  using std::unique_ptr;
#include <string>
  using std::string;
#include <fstream>
  using std::ifstream;

libambrosia_namespace_begin

project::project(const string& name, ambrosia_config& configuration)
: target(target_type::project, name, configuration),
  m_targets(),
  m_commands()
{   }

// Filter target_list and sort the targets that need to be built.
void project::apply_target_configuration()
{
  const map_string_set_string target_config = configuration->target_config_options();

  for(auto&& it = std::begin(target_config); it != std::end(target_config); ++it)
  {

  }
  throw error("project::apply_target_configuration::Not yet implemented.");
}

void project::sort_targets()
{
  debug(debug::dependencies) << "project::dependency_sort_targets::Pre-sort order:\n";
  std::for_each(std::begin(m_targets), std::end(m_targets),
                [](const target& t) { debug(debug::dependencies) << "\t" << t.name << "\n"; });
  /*target_vector resolved;
  while(!m_targets.empty())
  {
    dependency_resolve(std::begin(m_targets), resolved, m_targets);
  }

  m_targets.swap(resolved);*/
  //filter_dependency_sort(m_targets);
  debug(debug::dependencies) << "project::dependency_sort_targets::Post-sort order:\n";
  std::for_each(std::begin(m_targets), std::end(m_targets),
                [](const target& t) { debug(debug::dependencies) << "\t" << t.name << "\n"; });
}

libambrosia_namespace_end
