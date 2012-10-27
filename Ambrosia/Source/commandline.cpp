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
 * commandline.cpp
 * Function implementations.
 *
 **/

// Function includes
#include "commandline.h"

// Ambrosia includes
#include "help_and_version_output.h"
#include "output.h"
#include "program_options.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/configuration.h"
  using libambrosia::configuration;
#include "Ambrosia/debug.h"
  using libambrosia::debug;
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/commandline_error.h"
  using libambrosia::commandline_error;
#include "Ambrosia/Error/error.h"
  using libambrosia::error;
#include "Ambrosia/Error/soft_error.h"
  using libambrosia::soft_error;
#include "Ambrosia/file_cache.h"
  using libambrosia::file_cache;
#include "Ambrosia/nectar.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/Targets/project.h"
  using libambrosia::project;
#include "Ambrosia/status.h"

// C-ish includes
#include <cassert>

// C++ includes
#include <algorithm>
#include <cstddef>
  using std::size_t;
#include <sstream>
  using std::istringstream;
#include <string>
  using std::string;

ambrosia_namespace_begin

void apply_commandline_options(const string_vector& arguments,
                               program_options& options,
                               lib::project& project)
{
  // Debug output
  std::for_each(std::begin(arguments), std::end(arguments),
                [](const string& option)
                { static int i=1;
                  debug(debug::type::commandline) << "commandline::apply_commandline_options::argument: " << i++ << ": " << option << ".\n"; });
  debug(debug::commandline) << "commandline::apply_commandline_options::Number of commandline arguments: " << arguments.size() << ".\n";

  // Execution ending arguments
  if(lib::contains(arguments, string("-v")) || lib::contains(arguments, "-version"))
    throw soft_error("Execution stopped after request for version output.");
  else if(lib::contains(arguments, "-h") || lib::contains(arguments, "-help"))
  {
    print_help_information();
    throw soft_error( "Execution stopped after request for help output." );
  }

  // Options to be filled in, with default values, if any
  size_t argument_number = 0;
  bool m_first_dashless_argument = true;
  for(auto&& it = std::begin(arguments); it != std::end(arguments); ++it)
  {
    argument_number++;
    const string& current = *it;
    string::size_type index = current.find_first_not_of("-:");
    switch(index)
    {
      case 0:
        // only allow the first dashless argument as source dir or file path
        if(m_first_dashless_argument)
        {
          m_first_dashless_argument = false;
          debug(debug::commandline) << "commandline::apply_commandline_options::Possible project file or directory: \'" << current << "\'.\n";

          if(lib::platform::file_exists(current))
          {
            debug(debug::commandline) << "commandline::apply_commandline_options::Project file given on commandline.\n";
            project.configuration.project_file = current;
            continue;
          }
          else if(lib::platform::directory_exists(current))
          {
            debug(debug::commandline) << "commandline::apply_commandline_options::Project file given on commandline.\n";
            project.configuration.project_file = lib::find_project_file(current, project.configuration);
            continue;
          }

          lib::emit_warning("No source directory specified.");

          project.configuration.project_file = lib::find_project_file(".", project.configuration);
          if(project.configuration.project_file.empty())
          {
            debug(debug::commandline) << "commandline::Project file found in current directory \'.\': " << project.configuration.project_file << ".\n";
            lib::emit_warning("Ambrosia does not recommend an in-source build.");
          }
          else
            throw error("No project file found in specified path or current directory.");

          goto add_target;
        }
        else
        {
          add_target:
          const string target = current;
          string_set config_strings;
          if((it+1) != std::end(arguments) && *(it+1) == ":")
          {
            const string& list_of_options = *(++it);
            ++argument_number;
            do
            {
              const string::size_type previous_index = index;
              string::size_type index = list_of_options.find(',');
              const string option = list_of_options.substr(previous_index, index);
              if(!config_strings.insert(option).second)
                throw commandline_error("Duplicate config option to target " + target + ": " + option, argument_number);
            } while(index != string::npos);
          }
          add_build_target(options, target, config_strings);
        }
        break;
      case 1:
        if(current[0] == '-')
        {
          const string::size_type index = current.find("=",1);
          if(index == string::npos || index == current.size()-1)
            set_program_option(options, current.substr(1), argument_number);
            //throw commandline_error("Ambrosia internal options must be set by \'-option=value\' type arguments.", argument_number);

          const string option(current.substr(1,index-1));
          const string value(current.substr(index+1, string::npos));
          set_ambrosia_option(project, option, value, argument_number);
        }
        else if(current[0] == ':')
          add_configuration_options(current.substr(1), project.configuration);
        break;
      case 2:
        // TODO: user arguments defined in project file:
        //  --<argument>=<value>
        //  store in a string_map?
        break;
      default:
        throw commandline_error("Invalid commandline argument: " + current, argument_number);
    }
  }
  debug(debug::commandline) << "commandline::apply_commandline_options::Checking if project file was found.\n";
  // Ensure that a valid project file has been found
  if(!lib::platform::file_exists(lib::full_directory_name(project.configuration.source_directory, project.configuration.project_file)))
    throw lib::error("No project file specified on the commandline, nor was one found in the current directory.");
}

void add_build_target(program_options& options,
                      const string& target,
                      const string_set& config_strings)
{
  debug(debug::commandline) << "commandline::add_build_target::Target to be built: " << target << ".\n";
  options.target_options.insert(std::make_pair(target, config_strings));
}

void set_program_option(program_options& options,
                        const string& option,
                        const size_t argument_number)
{
  debug(debug::commandline) << "commandline::set_program_option::" << option << " without value being set.\n";
  if("dump-commands" == option)
    options.dump_commands = true;
  else
    throw commandline_error("Unknown option: " + option, argument_number);
}

void set_ambrosia_option(lib::project& project,
                         const std::string& option,
                         const std::string& value,
                         const size_t argument_number )
{
  debug(debug::commandline) << "commandline::set_internal_value_option::" << option << " with value \'" << value << "\' being set.\n";

  if("cross" == option)
  {
    debug(debug::commandline) << "commandline::set_internal_option::Cross-compiling for " << value << ".\n";
    set_ambrosia_cross(project.configuration, value, argument_number);
  }
#ifdef AMBROSIA_DEBUG
  else if("d" == option || "debug" == option)
  {
    debug(debug::always) << "commandline::set_internal_option::setting debug options: " << value << ".\n";
    string::size_type index = 0;
    string::size_type previous_index = 0;
    debug::type new_debug_level = static_cast<debug::type>(0);
    int loop=0;
    do {
      ++loop;
      index=value.find(',', previous_index);
      const string item = value.substr(previous_index, index-previous_index);
      if(item.empty())
        throw commandline_error("Trailing comma in argument to -" + option, argument_number);

      debug::type item_enum;
      if(!map_value(lib::debug_map, item, item_enum))
        throw commandline_error("Unknown debug type: " + item, argument_number);

      debug(debug::always) << "commandline::set_internal_option::enabling " << item << " debug output.\n";
      // add the debug output option
      new_debug_level = static_cast<debug::type>(new_debug_level ^ item_enum);
      previous_index = index+1;
    } while(index != string::npos);
    // set new debug level
    debug::s_level = new_debug_level;
  }
#endif // AMBROSIA_DEBUG
  else if("gnu-prefix" == option)
  {
    debug(debug::commandline) << "commandline::set_internal_option::Cross-compiling with GNU prefix " << value << ".\n";
    project.configuration.gnu_prefix = value + '-'; // add the prefix dash here so you get for example "x86_64-linux-gnu-gcc"
  }
  else
    throw commandline_error("Unknown option passed to Ambrosia: \n\t-" + option + "=" + value, argument_number);
}

void set_ambrosia_cross(lib::configuration& configuration,
                        const std::string& cross,
                        const size_t argument_number)
{
  debug(debug::config) << "configuration::Checking and setting cross-compilation options through Ambrosia specification.\n";

  // verify format
  if(!lib::wildcard_compare("*-*-*", cross))
    throw commandline_error("Ambrosia cross-compile specification should be of the form \'OS-Architecture-Toolchain\'.\n", argument_number);
  else
    debug(debug::config) << "configuration::cross has correct format.\n";

  // find relevant parts and complain if somethin's wrong
  const string::size_type architecture_index = cross.find("-") + 1;
  const string::size_type toolchain_index = cross.find("-", architecture_index) + 1;

  // split up the string
  const string os_string(cross.substr(0, architecture_index-1));
  const string architecture_string(cross.substr(architecture_index, toolchain_index-architecture_index-1));
  const string toolchain_string(cross.substr(toolchain_index, string::npos));
  debug(debug::config) << "configuration::cross options specified:\n"
                       << "              os = " << os_string << ".\n"
                       << "              architecture = " << architecture_string << ".\n"
                       << "              toolchain = " << toolchain_string << ".\n";

  // set the appropriate internal options
  if(!lib::map_value(lib::os_map, os_string, configuration.target_os))
    throw commandline_error("Specified invalid target OS: " + os_string, argument_number);

  if(lib::map_value(lib::architecture_map, architecture_string, configuration.target_architecture))
    throw commandline_error("Specified invalid target bitness: " + architecture_string, argument_number);

  if(lib::map_value(lib::toolchain_map, toolchain_string, configuration.target_toolchain))
    throw commandline_error("Specified invalid target toolchain: " + toolchain_string, argument_number);
}

bool add_configuration_options(const string& options,
                               lib::configuration& /*config*/)
{
  debug(debug::commandline) << "commandline::add_configuration_options::Target configuration option: " << options << " set.\n";
  // put them in a set
  istringstream stream(options);
  string temp;
  string_set new_options;
  while(getline(stream, temp, ','))
  {
    if(new_options.insert(temp).second == false)
      output() << "Warning: duplicate configuration option: " << temp << ".\n";
  }
  throw error("commandline::add_configuration_options is not finished yet.");
}



ambrosia_namespace_end
