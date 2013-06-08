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
#include "commandline.h++"
#include "help_and_version_output.h++"

// libAmbrosia includes
#include "Ambrosia/configuration.h++"
#include "Ambrosia/debug.h++"
#include "Ambrosia/dependency_paths.h++"
#include "Ambrosia/enum_maps.h++"
#include "Ambrosia/Error/error.h++"
#include "Ambrosia/Error/soft_error.h++"
#include "Ambrosia/nectar.h++"
#include "Ambrosia/program_options.h++"
#include "Ambrosia/Target/project.h++"

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

//#ifdef AMBROSIA_DEBUG
time_t begin_time;
time_t project_read_time;
time_t command_generation_time;
time_t command_execution_time;
void output_execution_times()
{
  cerr << "Ambrosia execution times:\n"
       << "Project loading: " << difftime(project_read_time, begin_time) << " seconds,\n"
       << "Command generation: " << difftime(command_generation_time, project_read_time) << " seconds,\n"
       << "Command execution: " << difftime(command_execution_time, command_generation_time) << " seconds,\n"
       << "Total build time: " << difftime(time(0), begin_time) << " seconds.\n";
}
//#endif

int main(int argc, char* argv[])
try {
//#ifdef AMBROSIA_DEBUG
  begin_time = time(0);
  project_read_time = begin_time;
  command_generation_time = begin_time;
  command_execution_time = begin_time;
//#endif
  // improve iostream performance
  std::ios_base::sync_with_stdio(false);

  // Welcome message
  print_version_information();

  lib::program_options options;
  lib::configuration configuration;
  lib::dependency_paths_set external_dependencies;
  lib::project project(configuration);

  string_vector arguments = lib::platform::get_commandline_arguments(argv, argc);
  apply_commandline_options(string_vector(argv+1, argv+argc), options, external_dependencies, project);

  lib::drink_nectar(project, external_dependencies);

//#ifdef AMBROSIA_DEBUG
  project_read_time = time(0);
//#endif

  project.generate_commands();

  if(options.dump_commands)
    project.dump_commands();

//#ifdef AMBROSIA_DEBUG
  command_generation_time = time(0);
//#endif

  project.execute_build_commands();

//#ifdef AMBROSIA_DEBUG
  command_execution_time = time(0);
//#endif

//#ifdef AMBROSIA_DEBUG
  output_execution_times();
//#endif
}
#ifdef AMBROSIA_DEBUG
catch(libambrosia::soft_error& e)
{
  e.output_message();
  #else
catch(libambrosia::soft_error&)
{
#endif
  output_execution_times();

}
catch(libambrosia::error& e)
{
  e.output_message();
//#ifdef AMBROSIA_DEBUG
  output_execution_times();
//#endif
}
catch(std::exception& e)
{
  cout << "something bad happened:\n";
  cout << e.what() << "\n";
//#ifdef AMBROSIA_DEBUG
  output_execution_times();
//#endif
}
