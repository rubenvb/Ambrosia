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
#include "Ambrosia/global.h++"

// libAmbrosia includes
#include "Ambrosia/Target/external.h++"
#include "Ambrosia/Target/target.h++"

namespace ambrosia
{
namespace lib
{

class binary : public target
{
public:
  // project local target
  binary(const std::string& name,
         const target_type type,
         const std::string& build_type,
         const std::string& linkage,
         const ::libambrosia::configuration& configuration,
         const std::unordered_map<file_type, file_set>& files = {},
         const std::map<file_type, string_set>& directories = {},
         const dependency_map& dependencies = {});

  void generate_commands();
  void dump_commands() const;
  void execute_build_commands() const;

  const std::string& source_directory() const
  { return configuration.source_directory; }
  void gather_dependency_information(string_set& header_directories,
                                     string_set& library_directories,
                                     string_vector& libraries);

  libambrosia::configuration configuration;

  dependency_map dependencies;

private:
  void generate_parallel_commands(const toolchain_option_map& toolchain_options,
                                  const os_option_map& os_options,
                                  const string_set& header_directories);
  void generate_final_commands(const string_set& library_directories,
                               const string_vector& dependency_libraries);
};

} // namespace lib

} // namespace ambrosia

#endif // AMBROSIA_TARGET_BINARY_H
