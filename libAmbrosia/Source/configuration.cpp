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
 * Ambrosia/configurationuration/configuration.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/configuration.h"

// libambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/enums.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/platform.h"

// C++ includes
#include <string>
  using std::string;

namespace ambrosia
{
namespace lib
{

configuration::configuration()
: environment_PATH(platform::get_environment_PATH()),
  build_architecture(platform::build_architecture),
  build_os(platform::build_os),
  build_toolchain(platform::detect_toolchain()),
  target_architecture(build_architecture),
  target_os(build_os),
  target_toolchain(build_toolchain),
  source_directory(),
  project_file(),
  build_directory("."),
  user_variables(),
  defines(),
  config_strings(),
  build_linkages({linkage::link_time, linkage::load_time}),
  build_types({build_type::debug, build_type::release})
{
  initialize_config();
  debug(debug::config) << "\nconfiguration::config_strings contains:\n" << config_strings << "\n";
}

void configuration::set_source_directory(const string& source_directory)
{
  if(!platform::directory_exists(source_directory))
    throw internal_error("Attempting to call configuration::set_source_directory with a non-existent directory: " + source_directory);

  debug(debug::config) << "configuration::set_source_directory::Setting source directory to: " << source_directory << "\n";
  this->source_directory = source_directory;
  debug(debug::config) << "configuration::set_source_directory::Adding " << source_directory << " to s_file_store.\n";
}

void configuration::initialize_config()
{
  config_strings =
    {
       os_map_inverse.at(target_os) , // Target OS
       architecture_map_inverse.at(target_architecture) , // Target architecture
       toolchain_map_inverse.at(target_toolchain) , // Toolchain
       "build_" + os_map_inverse.at(build_os) , // Build OS
       "build_" + architecture_map_inverse.at(build_architecture) , // Build architecture
      //entry_begin environment_map_inverse.at(build_environment) entry_end // Shell environment
       "debug" , // debug build is the default
    };
  // Convenience config strings
  // OS related
  if(target_os == os::Linux || target_os == os::MacOSX)
    config_strings.insert("unix");

  // Toolchain related
  // nada

  // Architecture related
  // nada

  // OS and toolchain related
  if(target_os == os::Windows && target_toolchain == toolchain::GNU)
      config_strings.insert("mingw");
}

const string& configuration::languagestd_option(const language_option_map& language_options,
                                                const file_type type) const
{
  switch(type)
  {
    case file_type::source_c:
    {
      static string_set cstd_strings = {"C89", "C90", "C11", "GNU89", "GNU90", "GNU99", "GNU11"};
      string_vector result;
      std::set_intersection(std::begin(cstd_strings), std::end(cstd_strings),
                            std::begin(config_strings), std::end(config_strings),
                            std::back_inserter(result));
      if(result.size() == 1)
        return language_options.at(language_option_config_map.at(result.back()));
      else if(result.size() > 1)
        throw internal_error("Double language std specification: ", result);
      else
        return language_options.at(language_option::std_c99);
    }
    case file_type::source_cxx:
    {
      static string_set cxxstd_strings = {"C++98", "C++03", "C++11", "GNU++98", "GNU++03", "GNU++11"};
      string_vector result;
      std::set_intersection(std::begin(cxxstd_strings), std::end(cxxstd_strings),
                            std::begin(config_strings), std::end(config_strings),
                            std::back_inserter(result));
      if(result.size() == 1)
        return language_options.at(language_option_config_map.at(result.back()));
      else if(result.size() > 1)
        throw internal_error("Double language std specification: ", result);
      else
        return language_options.at(language_option::std_cxx11);
    }
    default:
      throw internal_error("language std options not fully implemented yet.");
  }
}

} // namespace lib

} // namespace ambrosia
