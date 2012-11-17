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
 * Ambrosia/ambrosia.h
 * Ambrosia main interface file.
 *
 **/


#ifndef AMBROSIA_H
#define AMBROSIA_H

inline void generate_commands()
{
  // assume targets are in the correct dependent order
  for(auto&& target_it = std::begin(m_targets); target_it != std::end(m_targets); ++target_it)
  {
    target& current = **target_it;
    if(current.m_type == target_type::global)
    {
      debug(debug::command_gen) << "project::generate_commands::Skipping generation of build commands for target: " << current.name << ".\n";
      continue;
    }
    debug(debug::command_gen) << "project::generate_commands::Generating build commands for target: " << current.name << "\n"
                              << "\tfor output in this directory: " << current.m_configuration.m_build_directory << "\n"
                              << "\tfor the following types of source files:\n"
                              << "\t" << current.m_configuration.m_source_types << "\n";

    for(auto&& type_it = std::begin(current.m_configuration.m_source_types); type_it != std::end(current.m_configuration.m_source_types); ++type_it)
    {
      const auto& type = *type_it;
      debug(debug::command_gen) << "project::generate_commands::Generating commands for " << current.files(type).size() << " "
                                << file_type_map_inverse.at(type) << " files.\n";
      //TODO generalize to a "get_generator function when there are languages supported that need a different style of processing
      unique_ptr<generator> generator(new compile_and_link_generator(type, current));

      generator->generate_object_filenames();
      const string_vector commands = generator->generate_parallel_commands();

      //TODO: debug output of commands, or storage in a per-target list for nice output.

      //TODO: fix ugly function call below
      m_commands[current.name].insert(std::end(m_commands[current.name]), std::begin(commands), std::end(commands));
    }
  }
}

inline void execute_build_commands()
{

}


#endif // AMBROSIA_H
