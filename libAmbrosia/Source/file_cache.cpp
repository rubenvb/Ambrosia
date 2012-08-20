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
 * Ambrosia/file_cache.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/file_cache.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Configuration/ambrosia_config.h"
#include "Ambrosia/debug.h"
#include "Ambrosia/Error/error.h"
#include "Ambrosia/platform.h"

// C++ includes
#include <stdexcept>
  using std::logic_error;
#include <string>
  using std::string;
#include <utility>
  using std::pair;

libambrosia_namespace_begin

//TODO:REMOVE
file_cache s_file_cache;

file_cache::file_cache()
: m_source_files(),
  m_build_files()
{   }

const string file_cache::find_nectar_file(const string& directory,
                                          config_base* config)
{
  debug(debug::files) << "nectar::find_nectar_file called for: " << directory << ".\n";
  config->set_source_directory(directory);
  file_set candidates = find_source_file("*.nectar.txt", config);
  switch(candidates.size())
  {
    case 0:
      throw error("No *.nectar.txt file found in " + directory);
    case 1:
      return std::begin(candidates)->first; // first = filename, second = modified
    default:
      throw error("Multiple *.nectar.txt files found in directory: " + directory, candidates);
  }
  return string("");
}

bool file_cache::find_project_file(const string& path,
                                   config_base* config)
{
  debug(debug::files) << "nectar::find_project_file::Called for " << path << ".\n";

  if(lib::file_exists(path))
  {
    debug(debug::files) << "nectar::find_project_file::Detected file.\n";
    // TODO: generalize the directory seperators list
    // TODO: seperate filename from (relative) path
    const string::size_type index = path.find_last_of("/\\"); // breaks for Unix paths with spaces (maybe)
    config->set_project_file(path.substr(index+1, string::npos));
    config->set_source_directory(path.substr(0, index));
  }
  else if(lib::directory_exists(path))
  {
    debug(debug::files) << "nectar::find_project_file detected directory.\n";
    const string project_file = find_nectar_file(path, config);
    // if the directory contains a *.nectar.txt file, set source directory as well
    if(!project_file.empty())
    {
      debug(debug::files) << "nectar::Project file found: " << project_file << ".\n";
      config->set_source_directory(path);
      config->set_project_file(project_file);
      return true;
    }
  }
  // return failure if some condition failed
  debug(debug::files) << "nectar::No *.nectar.txt file found in " << path << ".\n";
  return false;
}

const file_set& file_cache::get_source_file_set(const std::string& directory)
{
  debug(debug::files) << "file_set::get_source_file_set::Finding directory listing for " << directory << ".\n";
  const auto result = m_source_files.find(directory);
  if(result != std::end(m_source_files))
    return m_source_files[directory];
  else
  {
    debug(debug::files) << "file_cache::get_source_file_set::Directory not found, scanning now.\n";
    add_source_directory(directory);
    return m_source_files[directory];
  }
}

const file_set file_cache::find_source_file(const string& filename,
                                            const config_base* config,
                                            const string_set& directories )
{
  debug(debug::files) << "file_cache::find_source_file::Called.\n";
  const string& source_directory = config->m_source_directory;
  // handle filename with directory prepended
  const string_pair directory_filename(split_preceding_directory(filename));
  const string& preceding_directory = directory_filename.first;
  const string& true_filename = directory_filename.second;
  // handle empty "directories" case
  string_set directories_to_search;
  if(directories.empty())
    directories_to_search.insert(source_directory);
  else
  {
    std::for_each(std::begin(directories), std::end(directories), [&](const string& directory)
                  {
                    const string full_dir = full_directory_name(source_directory, full_directory_name(directory, preceding_directory));
                    if(directory_exists(full_dir))
                    {
                      debug(debug::files) << "file_cache::find_source_file::Adding deduced directory to search list: " << full_dir << "\n";
                      directories_to_search.insert(full_dir);
                    }
                    else
                    debug(debug::files) << "file_cache::find_source_file::Not adding non-existing deduced directory to search list: " << full_dir << ".\n";
                  });
    if(directories_to_search.empty())
      directories_to_search.insert(source_directory);
  }
  debug(debug::files) << "file_cache::find_source_file::Looking for " << filename << " in the following subdirectories of " << source_directory << ":\n"
                      << directories_to_search;

  file_set result;

  for(auto&& it = std::begin(directories_to_search); it != std::end(directories_to_search); ++it)
  {
    const string& directory = *it;

    debug(debug::files) << "file_cache::find_source_file::Loading directory contents for: " << directory << ".\n";
    const file_set& files_on_disk = get_source_file_set(directory);

    for(auto&& it = std::begin(files_on_disk); it != std::end(files_on_disk); ++it)
    {
      const file& entry = *it;
      debug(debug::files) << "file_cache::find_source_file::Matching " << entry.first << " vs " << true_filename << ".\n";
      if(wildcard_compare(true_filename, entry.first))
      {
        debug(debug::files) << "file_cache::find_source_file::Match found: " << entry.first << "\n";
        result.insert({directory + "/" + entry.first, entry.second});
      }
    }
  }
  debug(debug::files) << "file_cache::find_source_file::Found " << result.size() << " match(es).\n";
  return result;
}
const file_set file_cache::match_source_files(const string& filename,
                                              const config_base* config,
                                              const string_set& directories)
{
  debug(debug::files) << "file_cache::match_source_files::Matching " << filename
  << " to all files in the source directories.\n";
  file_set result;
  const string_pair directory_filename(split_preceding_directory(filename));
  const string& preceding_directory(directory_filename.first);
  const string& true_filename( directory_filename.second );

  // search all directories, appended with preceding_directory
  for(auto&& directory_it = std::begin(directories); directory_it != std::end(directories); ++directory_it)
  {
    const string directory(full_directory_name(config->m_source_directory, *directory_it + preceding_directory));
    if(!directory_exists(directory))
    {
      debug(debug::files) << "file_cache::match_source_files::Skipping nonexistent directory: " << directory << ".\n";
      continue;
    }
    debug(debug::files) << "file_cache::match_source_files::Looking in " << directory << " for matches.\n";

    const file_set& files_on_disk = get_source_file_set(directory);

    debug(debug::files) << "file_cache::match_source_files::Searching for match with " << files_on_disk.size() << " files.\n";

    // match all files that were scanned from disk to the wildcard filename
    for(auto&& files_it = std::begin(files_on_disk); files_it != std::end(files_on_disk); ++files_it)
    {
      const file& entry = *files_it; // filename and last modified time
      debug(debug::files) << "file_cache::match_source_files::Matching " << entry.first << " with " << true_filename << ".\n";
      if(wildcard_compare(true_filename, entry.first))
      {
        debug(debug::files) << "file_cache::match_source_files::Matched " << true_filename << " to " << directory << "/" << entry.first << ".\n";
        result.insert({directory + "/" + entry.first, entry.second});
      }
    }
  }
  debug(debug::files) << "file_cache::match_source_files::Found " << result.size() << " matches.\n";
  return result;
}

void file_cache::add_source_directory(const string& directory)
{
#ifdef AMBROSIA_DEBUG
  if(!directory_exists(directory))
    throw logic_error("Directory does not exist: " + directory);
#endif

  debug(debug::files) << "file_cache::add_source_directory::Scanning files in source directory: " << directory << ".\n";
  const auto result = m_source_files.insert({directory, file_set()});
  if(!result.second)
    debug(debug::files) << "file_cache::add_source_directory::Directory already present, and scanned.\n";
  else
  {
    file_set& new_files = result.first->second;
    scan_directory(std::inserter(new_files, std::begin(new_files)), directory);
    debug(debug::files) << "file_cache::add_source_directory::Directory scanned.\n";
  }
}
void file_cache::add_build_directory(const std::string& directory)
{
  if(!directory_exists(directory))
    // TODO: Create directory so scan_directory works OK. Check if creation was possible.
    debug(debug::files) << "file_cache::add_build_direcctory::Creating build directory: " << directory << ".\n";

  debug(debug::files) << "file_cache::add_build_directory::Scanning files in build directory: " << directory << ".\n";
  const auto result = m_build_files.insert({directory, file_set()});
  if(!result.second)
    debug(debug::files) << "file_cache::add_source_directory::Directory already present, and scanned.\n";
  else
  {
    file_set& new_files = result.first->second;
    scan_directory(std::inserter(new_files, std::begin(new_files)), directory);
  }
}

libambrosia_namespace_end
