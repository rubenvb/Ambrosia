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
 * Ambrosia/Targets/target.h
 * Abstract target base class.
 *
 **/

#ifndef AMBROSIA_TARGETS_TARGET_H
#define AMBROSIA_TARGETS_TARGET_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class target
{
public:
  target(const configuration& configuration,
         const std::string& name = "",
         const dependency_set& dependencies = dependency_set());

  bool add_source_directory(const file_type type,
                            const std::string& directory);
  void add_source_file(const file_type type,
                       const std::string& filename,
                       const std::string& nectar_file,
                       const std::size_t line_number);

  bool add_library(const std::string& library,
                   const std::string& nectar_file,
                   const std::size_t line_number);

  const build_element_set& files(const file_type type) const
  { return m_files.at(type); }
  const string_set source_directories(const file_type type) const
  { return m_source_directories.at(type); }

  const std::string name;
  lib::configuration configuration;
  const dependency_set dependencies;

private:
  std::map<file_type, build_element_set> m_files;
  std::map<file_type, string_set> m_source_directories;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_TARGET_H
