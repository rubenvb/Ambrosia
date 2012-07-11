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

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
  using libambrosia::ambrosia_config;
#include "Ambrosia/debug.h"
  using libambrosia::debug;
#include "Ambrosia/Error/commandline_error.h"
  using libambrosia::commandline_error;
#include "Ambrosia/Error/error.h"
  using libambrosia::error;
#include "Ambrosia/Error/soft_error.h"
  using libambrosia::soft_error;
#include "Ambrosia/file_cache.h"
  using libambrosia::file_cache;
#include "Ambrosia/nectar.h"
#include "Ambrosia/project.h"
  using libambrosia::project;
#include "Ambrosia/Error/error.h"

// C-ish includes
#include <cassert>

// C++ includes
#include <algorithm>
#include <sstream>
  using std::istringstream;
#include <string>
  using std::string;

ambrosia_namespace_begin

void apply_commandline_options(const string_vector& arguments,
                               lib::file_cache& files)
{
  // Debug output
  std::for_each(arguments.begin(), arguments.end(),
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
  for(auto it = arguments.begin(); it != arguments.end(); ++it)
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
          debug(debug::commandline) << "begin::Possible project file or directory: \'" << current << "\'.\n";

          files.find_project_file(current, lib::project::configuration);

          // if project_file is still empty, "current" is really a target name
          //  to be built, skip to below next else
          if(lib::project::configuration->project_file().empty())
            goto add_target;
        }
        else
        {
          add_target:
          const string target = current;
          string_set options;
          if((it+1) != arguments.end() && *(it+1) == ":")
          {
            const string& list_of_options = *(++it);
            ++argument_number;
            do
            {
              const string::size_type previous_index = index;
              string::size_type index = list_of_options.find(',');
              const string option = list_of_options.substr(previous_index, index);
              if(!options.insert(option).second)
                throw commandline_error("Duplicate config option to target " + target + ": "
                                        + option, argument_number);
            } while(index != string::npos);
          }
          add_build_target(target, options);
        }
        break;
      case 1:
        if(current[0] == '-')
        {
          const string::size_type index = current.find("=",1);
          if(index == string::npos || index == current.size()-1)
            throw commandline_error("Ambrosia internal options must be set by \'-option=value\' type arguments.", argument_number);

          const string option(current.substr(1,index-1));
          const string value(current.substr(index+1, string::npos));
          set_internal_option(option, value, argument_number);
        }
        else if(current[0] == ':')
          add_configuration_options(current.substr(1), project::configuration);
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
  // if project file is not yet set, search current directory
  if(project::configuration->project_file().empty())
  {
    if(files.find_project_file(".", project::configuration))
    {
      debug(debug::commandline) << "commandline::Project file found in current directory \'.\': " << project::configuration->project_file() << ".\n";
      lib::emit_warning("Ambrosia does not recommend an in-source build.");
    }
    else
      throw error("No project file found in specified path or current directory.");
  }
  debug(debug::commandline) << "commandline::apply_commandline_options::Checking if project file was found.\n";
  // Ensure that a valid project file has been found
  assert(lib::file_exists(lib::project::configuration->project_file()));
}

void add_build_target(const string& target, const string_set& options)
{
  // TODO: fixme: this function does wrong things
  const string::size_type index = target.find(":");
  if(index == string::npos)
  {
    debug(debug::commandline) << "commandline::add_build_target::Target to be built: " << target << ".\n";
    project::configuration->add_target_config_options(target, options);
  }
  else
  {
    const string target_name(target.substr(0, index));
    debug(debug::commandline) << "commandline::add_build_target::Target to be built: " << target_name << ".\n";
    string_set options;
    string_set duplicates;
    // FIXME: stringstream --> string::find
    istringstream stream( target );
    stream.seekg( static_cast<istringstream::streamoff>(index) );
    string temp;
    while(std::getline(stream, temp, ','))
    {
      if(options.insert(temp).second == false)
        duplicates.insert( temp );
    }
    project::configuration->add_target_config_options(target_name, options);
  }
}

void set_internal_option(const std::string& option,
                         const std::string& value,
                         const size_t argument_number )
{
  debug(debug::commandline) << "commandline::set_internal_option::" << option << " with value \'" << value << "\' being set.\n";

  if("cross" == option)
  {
    debug(debug::commandline) << "commandline::set_internal_option::Cross-compiling for " << value << ".\n";
    project::configuration->set_ambrosia_cross(value, argument_number);
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
    project::configuration->set_gnu_prefix(value);
  }
  else
    throw commandline_error("Unknown option passed to Ambrosia: \n\t-" + option + "=" + value, argument_number);
}

bool add_configuration_options(const string& options,
                               ambrosia_config* /*config*/)
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
