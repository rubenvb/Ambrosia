/**
  * target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/target.h"

// Ambrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/enum_maps.h"
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
: node(subdirectory + "::global"),
  m_type(target_type::global),
  m_dependencies(dependencies),
  m_build_config(subdirectory, config),
  m_source_directories(),
  m_source_files(),
  m_libraries(),
  m_output_name(subdirectory + "::global")
{   }
target::target(const string& name,
               const target_type type,
               const dependency_set& dependencies,
               const build_config& config)
: node(name),
  m_type(type),
  m_dependencies(dependencies),
  m_build_config(config),
  m_source_directories(),
  m_source_files(),
  m_libraries(),
  m_output_name(name)
{
  debug(debug::target) << "target::Created " << map_value(target_type_map_inverse, type) << ": " << name << ".\n";
}

/*
 * Getters
 **********/
const string& target::name() const
{
  return m_name;
}
target_type target::type() const
{
  return m_type;
}
const dependency_set& target::dependencies() const
{
  return m_dependencies;
}
/*
 * Setters
 **********/
build_config& target::config()
{
  return m_build_config;
}
const build_config& target::config() const
{
  return m_build_config;
}

void target::add_source_file(const file_type type,
                             const string& filename,
                             const string& nectar_file,
                             const size_t line_number)
{
  if(contains(filename, "*?"))
  {
    const file_set matches = s_file_cache.match_source_files(filename, &m_build_config, m_source_directories[type]);
    if(matches.empty())
      return emit_nectar_error("No files matching " + filename + " found.", nectar_file, line_number);
    //FIXME: this should add files to m_source_files and check for ambiguities like below
  }
  else
  {
    const file_set matches = s_file_cache.find_source_file( filename, &m_build_config, m_source_directories[type]);
    switch(matches.size())
    {
      case 0:
        return emit_nectar_error("No matches to file " + filename + " found.", nectar_file, line_number);
      case 1:
        m_source_files[type].insert(*matches.begin());
        break;
      default:
        string_vector ambiguous;
        std::for_each(matches.begin(), matches.end(), [&ambiguous](const file& f) { ambiguous.push_back(f.first); });
        return emit_error_list({ambiguous});
    }
  }
  // add build config source type to determine which command generators need to be run
  m_build_config.m_source_types.insert(type); // no need to check failure: only needs to be present
}
void target::remove_file(const file_type type,
                         const string& filename)
{
  // search for file, check if there are any other files of the same type,
  //  and remove the "source file config" for generation phase.
  s_file_cache.match_source_files(filename, &m_build_config, m_source_directories[type]);
  emit_error("target::remove_file::Unimplementented.");
}
bool target::add_source_directory(const file_type type,
                                  const string& directory)
{
  const string full_subdirectory_name = full_directory_name(m_build_config.source_directory(), directory);

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
}
bool target::add_library(const string& library)
{
  debug(debug::target) << "target::add_library::Adding library " << library << " to target " << m_name << ".\n";
  //TODO: check if library can be linked
  return !(m_libraries.insert(library).second);
}
void target::remove_library(const string& library)
{
  if( m_libraries.erase(library) )
    emit_warning_list( {library} );
}
void target::set_output_name(const std::string& name)
{
  m_output_name = name;
}

/*
 * Private functions
 ********************/

libambrosia_namespace_end
