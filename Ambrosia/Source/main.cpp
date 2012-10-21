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
 * main.cpp
 * Ambrosia's main function.
 *
 **/

// Ambrosia includes
#include "commandline.h"
#include "help_and_version_output.h"
#include "program_options.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/Error/soft_error.h"
#include "Ambrosia/nectar.h"
#include "Ambrosia/Targets/project.h"

using namespace ambrosia;

// C-ish includes
#include <cstdlib>
#include <ctime>

// C++ includes
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
try {
#ifdef AMBROSIA_DEBUG
  time_t t = time(0);
#endif
  // improve iostream performance
  std::ios_base::sync_with_stdio(false);

  // Welcome message
  print_version_information();

  program_options options;
  lib::configuration configuration;
  lib::project project(configuration);

  apply_commandline_options(string_vector(argv+1, argv+argc), options, project);

  //project.read_project_files();
  lib::drink_nectar(project);
#ifdef AMBROSIA_DEBUG
  const double project_read_time = difftime(time(0), t);
  t = time(0);
#endif
  // project.apply_project_configuration();

  //project.sort_targets();

  //project.generate_commands();

  //if(lib::project::configuration->m_dump_commands)
  //  project.dump_commands();

#ifdef AMBROSIA_DEBUG
  const double command_generation_time = difftime(time(0), t);
  t = time(0);
#endif

  //project.execute_build_commands();

#ifdef AMBROSIA_DEBUG
  const double build_time = difftime(time(0), t);
  cerr << "Runtimes:\n"
       << "Project loading: " << project_read_time << " seconds,\n"
       << "Command generation: " << command_generation_time << " seconds,\n"
       << "Actual build time: " << build_time << "\n.";
#endif
}
#ifdef AMBROSIA_DEBUG
catch(libambrosia::soft_error& e)
{
  e.output_message();
#else
catch(libambrosia::soft_error&)
{
#endif
}
catch(libambrosia::error& e)
{
  e.output_message();
}
catch(std::exception& e)
{
  cout << "something bad happened:\n";
  cout << e.what() << "\n";
}
