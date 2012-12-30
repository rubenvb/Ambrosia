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
#include "Ambrosia/configuration.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/Targets/project.h"
#include "Ambrosia/typedefs.h"
#include "Ambrosia/nectar_loader.h"

// C++ includes
#include <fstream>
  using std::ifstream;
#include <iterator>
  using std::inserter;
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

const string find_project_file(const string& directory,
                               project& project)
{
  debug(debug::nectar) << "nectar::find_project_file called for: " << directory << ".\n";
  file_set candidates;
  platform::scan_directory(inserter(candidates, candidates.begin()), directory);

  // filter files on "*.nectar.txt"
  for(auto it = std::begin(candidates); it != std::end(candidates);)
  {
    debug(debug::nectar) << "nectar::find_project_file::checking " << it->name << ".\n";
    if(!wildcard_compare("*.nectar.txt", it->name))
      it = candidates.erase(it);
    else
      ++it;
  }
  debug(debug::nectar) << "nectar::find_project_file::Candidate matches: " << candidates.size() << ".\n";
  switch(candidates.size())
  {
    case 0:
      throw error("No *.nectar.txt file found in " + directory);
    case 1:
    {
      const string filename = std::begin(candidates)->name;
      debug(debug::nectar) << "Project file found: " << filename << ".\n";
      project.configuration.source_directory = directory;
      project.name = filename.substr(0,filename.size()-11); // Filename without ".nectar.txt" extension
      return filename;
    }
    default:
      throw error("Multiple *.nectar.txt files found in directory: " + directory, candidates);
  }
  return string("");
}

void drink_nectar(project& project)
{
  // open file
  const string& filename = project.configuration.source_directory / project.configuration.project_file;
  const auto& stream_ptr(platform::open_ifstream(filename));
  auto& stream = *stream_ptr;
  if(!stream)
    throw error("Unable to open *.nectar.txt file: " + filename);

  // read targets
  debug(debug::files) << "nectar::opening file: " << filename << " succeeded, loading contents.\n";
  nectar_loader loader(project, filename, "", stream);

  loader.extract_nectar();
  debug(debug::nectar) << "nectar::drink_nectar::Finished parsing project files.\n";
}

void apply_configuration(target_ptr_vector& /*targets*/)
{
  throw error("nectar::apply_configuration::Not implemented.");
}

libambrosia_namespace_end
