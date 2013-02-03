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
 * Ambrosia/Target/binary.h
 * Binary target, with a library or app as output.
 *
 **/

#ifndef AMBROSIA_TARGET_BINARY_H
#define AMBROSIA_TARGET_BINARY_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Target/external.h"
#include "Ambrosia/Target/target.h"

libambrosia_namespace_begin

class binary : public target
{
public:
  // project local target
  binary(const std::string& name,
         const ::libambrosia::configuration& configuration,
         const target_type type,
         const dependency_map& dependencies = dependency_map());

  // Target information
  bool add_source_directory(const file_type type,
                            const std::string& directory,
                            file_cache& file_cache);
  void add_source_file(const file_type general_type,
                       const std::string& filename,
                       file_cache& file_cache,
                       const std::string& nectar_file,
                       const std::size_t line_number);
  void add_library(const std::string& library,
                   const std::string& nectar_file,
                   const std::size_t line_number);

  void generate_commands();
  void dump_commands() const;
  void execute_build_commands() const;

  const std::string& source_directory() const
  { return configuration.source_directory; }

  libambrosia::configuration configuration;

  dependency_map dependencies;
};

libambrosia_namespace_end

#endif // AMBROSIA_TARGET_BINARY_H
