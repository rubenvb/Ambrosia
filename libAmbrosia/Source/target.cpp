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
 * target.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/target.h"

// Ambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/Error/nectar_error.h"
#include "Ambrosia/file_cache.h"
#include "Ambrosia/platform.h"
#include "Ambrosia/status.h"

// C++ includes
#include <stdexcept>
  using std::runtime_error;
#include <string>
  using std::string;
#include <utility>
  using std::pair;
#include <vector>
  using std::vector;

libambrosia_namespace_begin

target::target(const string& subdirectory,
               const dependency_set& dependencies,
               const ambrosia_config& config)
: node(subdirectory + "::global"), //TODO: fix for multilevel subdirectories
  m_build_config(subdirectory, config),
  m_dependencies(dependencies),
  m_output_name(),
  m_type(target_type::global),
  m_source_directories(),
  m_source_files(),
  m_libraries()
{   }
target::target(const string& subdirectory,
               const string& name,
               const target_type type,
               const dependency_set& dependencies,
               const build_config& config)
: node(subdirectory + "::" + name), //TODO: fix for multilevel subdirectories
  m_build_config(config),
  m_dependencies(dependencies),
  m_output_name(name),
  m_type(type),
  m_source_directories(),
  m_source_files(),
  m_libraries()
{
  debug(debug::target) << "target::Created " << map_value(target_type_map_inverse, type) << ": " << name << ".\n";
}

void target::add_source_file(const file_type type,
                             const string& filename,
                             const string& nectar_file,
                             const size_t line_number)
{
  if(contains(filename, "*?"))
  {
    // find file matches
    const file_set matches = s_file_cache.match_source_files(filename, &m_build_config, m_source_directories[type]);
    if(matches.empty())
      return emit_nectar_error("No files matching " + filename + " found.", nectar_file, line_number);

    // add matches, files already present cause error
    string_vector duplicates;
    for(auto it = matches.begin(); it != matches.end(); ++it)
    {
      const auto& current = *it;
      const file_type detected_type = detect_type(type, current.first);
      if(!m_source_files[detected_type].insert(current).second)
        duplicates.push_back(current.first);
      else
        m_build_config.m_source_types.insert(detected_type);
    }
    if(!duplicates.empty())
      throw nectar_error("Wildcard matches already added files: ", nectar_file, line_number, duplicates);
  }
  else
  {
    const file_set matches = s_file_cache.find_source_file(filename, &m_build_config, m_source_directories[type]);
    switch(matches.size())
    {
      case 0:
        return emit_nectar_error("No matches to file " + filename + " found.", nectar_file, line_number);
      case 1:
      {
        const file_type detected_type = detect_type(type, filename);
        m_source_files[detected_type].insert(*matches.begin());
        m_build_config.m_source_types.insert(detected_type);
        break;
      }
      default:
        string_vector ambiguous;
        std::for_each(matches.begin(), matches.end(), [&ambiguous](const file& f) { ambiguous.push_back(f.first); });
        throw nectar_error("Ambiguity in file selection: ", filename, line_number, ambiguous);
    }
  }
}
void target::remove_file(const file_type type,
                         const string& filename)
{
  // search for file, check if there are any other files of the same type,
  //  and remove the "source file config" for generation phase.
  s_file_cache.match_source_files(filename, &m_build_config, m_source_directories[type]);
  throw error("target::remove_file::Unimplementented.");
}
bool target::add_source_directory(const file_type type,
                                  const string& directory)
{
  const string full_subdirectory_name = full_directory_name(m_build_config.m_source_directory, directory);

  debug(debug::target) << "target::add_source_directory::Checking if directory " << full_subdirectory_name << " exists.\n";
  if(!directory_exists(full_subdirectory_name))
    return false;

  s_file_cache.add_source_directory(full_subdirectory_name);
  m_source_directories[type].insert(directory);
  return true;
}
void target::remove_directory(const file_type type,
                              const string& directory)
{
  if(m_source_directories[type].erase(directory))
    emit_warning_list({directory});

  throw error("target::remove_directory has a flaky implementation.");
}
bool target::add_library(const string& library)
{
  debug(debug::target) << "target::add_library::Adding library " << library << " to target " << m_name << ".\n";
  //TODO: check if library can be linked
  return !(m_libraries.insert(library).second);
}
void target::remove_library(const string& library)
{
  if(m_libraries.erase(library))
    emit_warning_list( {library} );

  throw error("remove_library has flaky implementation.");
}
const file_set& target::source_files(const file_type type) const
{
  const auto it = m_source_files.find(type);
  if(it == m_source_files.end() || (*it).second.empty())
  {
    debug(debug::target) << "target::source_files::Attempt at getting at source file_set for " << map_value(file_type_map_inverse, type)
                         << " which is either nonexistent or empty.\n";
    throw internal_error("target::source_files called with a file_type for which no source files are present.");
  }
  return (*it).second;
}

libambrosia_namespace_end
