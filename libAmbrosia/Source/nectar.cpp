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
 * Ambrosia/nectar.cpp
 * Class implementation.
 *
 **/

// Function include
#include "Ambrosia/nectar.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/project.h"
#include "Ambrosia/typedefs.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
#include <fstream>
  using std::ifstream;
#include <iterator>
  using std::back_insert_iterator;
#include <memory>
  using std::unique_ptr;
#include <set>
  using std::set;
#include <string>
  using std::string;
#include <vector>
  using std::vector;

// C-ish includes
#include <ctime>

libambrosia_namespace_begin

void drink_nectar(project& project,
                  const ambrosia_config& configuration,
                  file_cache& file_cache)
{
  // open file
  const string& filename = configuration.project_file;
  const auto&& stream_ptr(open_ifstream(filename));
  auto&& stream = *stream_ptr;
  if(!stream)
    throw error("Unable to open *.nectar.txt file: " + filename);

  // read targets
  debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
  nectar_loader loader(project, filename, "", stream);

  loader.extract_nectar(configuration, file_cache);
  debug(debug::nectar) << "nectar::drink_nectar::Finished parsing project files.\n";
}

void apply_build_config(target_ptr_vector& /*targets*/)
{
  throw error("nectar::apply_build_config::Not implemented.");
}

libambrosia_namespace_end
