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
#include "Ambrosia/file.h"
#include "Ambrosia/configuration.h"
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

file_cache::file_cache()
: m_source_files(),
  m_build_files()
{   }

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

void file_cache::find_source_files(const std::string& filename,
                                   const std::string& source_directory,
                                   const string_set& subdirectories,
                                   build_element_set& files)
{
  debug(debug::files) << "file_cache::find_source_files::Finding matches for " << filename << " in subdirectories of \'" << source_directory << "\':\n" << subdirectories;
  // handle filename with directory prepended
  const string_pair directory_filename(split_preceding_directory(filename));
  const string& preceding_directory = directory_filename.first;
  const string& true_filename = directory_filename.second;
  for(auto&& subdirectory : subdirectories)
  {
    const string full_directory = source_directory / subdirectory / preceding_directory;
    debug(debug::files) << "file_cache::find_source_files::Finding matches in " << full_directory << ".\n";
    if(!add_source_directory(full_directory))
    {
      debug(debug::files) << "file_cache::find_source_files::Skipping nonexistent directory: " << full_directory << ".\n";
      continue;
    }
    const file_set& sources = get_source_file_set(full_directory);
    for(auto&& source_file : sources)
    {
      debug(debug::files) << "file_cache::find_source_files::Comparing " << true_filename << " to " << source_file.name << ".\n";
      if(wildcard_compare(true_filename, source_file.name))
      {
        debug(debug::files) << "file_cache::find_source_files::Match found: " << source_file.name << "\n";
        files.insert(file(full_directory / source_file.name, source_file.time_modified));
      }
    }
  }
}

const file_set file_cache::match_source_files(const string& filename,
                                              const configuration* configuration,
                                              const string_set& directories)
{
  debug(debug::files) << "file_cache::match_source_files::Matching " << filename << " to all files in the source directories.\n";
  file_set result;
  const string_pair directory_filename(split_preceding_directory(filename));
  const string& preceding_directory(directory_filename.first);
  const string& true_filename(directory_filename.second);

  // search all directories, appended with preceding_directory
  for(auto&& directory : directories)
  {
    const string full_directory(configuration->source_directory / (directory + preceding_directory));
    if(!platform::directory_exists(full_directory))
    {
      debug(debug::files) << "file_cache::match_source_files::Skipping nonexistent directory: " << full_directory << ".\n";
      continue;
    }
    debug(debug::files) << "file_cache::match_source_files::Looking in " << full_directory << " for matches.\n";

    const file_set& files_on_disk = get_source_file_set(full_directory);

    debug(debug::files) << "file_cache::match_source_files::Searching for match with " << files_on_disk.size() << " files.\n";

    // match all files that were scanned from disk to the wildcard filename
    for(auto&& entry : files_on_disk)
    {
      debug(debug::files) << "file_cache::match_source_files::Matching " << entry.name << " with " << true_filename << ".\n";
      if(wildcard_compare(true_filename, entry.name))
      {
        debug(debug::files) << "file_cache::match_source_files::Matched " << true_filename << " to " << full_directory << "/" << entry.name << ".\n";
        result.insert(file(full_directory / entry.name, entry.time_modified));
      }
    }
  }
  debug(debug::files) << "file_cache::match_source_files::Found " << result.size() << " matches.\n";
  return result;
}

bool file_cache::add_source_directory(const string& directory)
{
  debug(debug::files) << "file_cache::add_source_directory::Checking if directory " << directory << " exists.\n";
  if(!platform::directory_exists(directory))
  {
    debug(debug::files) << "file_cache::add_source_directory::Nonexistent directory: " << directory << ".\n";
    return false;//throw logic_error("Directory does not exist: " + directory);
  }
  debug(debug::files) << "file_cache::add_source_directory::Scanning files in source directory: " << directory << ".\n";
  const auto result = m_source_files.insert(std::make_pair(directory, file_set()));
  if(!result.second)
    debug(debug::files) << "file_cache::add_source_directory::Directory already present, and scanned.\n";
  else
  {
    file_set& new_files = result.first->second;
    platform::scan_directory(std::inserter(new_files, std::begin(new_files)), directory);
    debug(debug::files) << "file_cache::add_source_directory::Directory scanned.\n";
  }
  return true;
}
void file_cache::add_build_directory(const std::string& directory)
{
  if(!platform::directory_exists(directory))
    // TODO: Create directory so scan_directory works OK. Check if creation was possible.
    debug(debug::files) << "file_cache::add_build_direcctory::Creating build directory: " << directory << ".\n";

  debug(debug::files) << "file_cache::add_build_directory::Scanning files in build directory: " << directory << ".\n";
  const auto result = m_build_files.insert(std::make_pair(directory, file_set()));
  if(!result.second)
    debug(debug::files) << "file_cache::add_source_directory::Directory already present, and scanned.\n";
  else
  {
    file_set& new_files = result.first->second;
    platform::scan_directory(std::inserter(new_files, std::begin(new_files)), directory);
  }
}

libambrosia_namespace_end
