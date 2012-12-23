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
 * Ambrosia/Targets/dummy.h
 * Dummy target class which allows a target definition syntax to be checked, but no actual files are checked.
 *
 **/

#ifndef AMBROSIA_TARGETS_DUMMY_H
#define AMBROSIA_TARGETS_DUMMY_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/configuration.h"
#include "Ambrosia/Targets/target.h"
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class dummy : public target
{
  dummy(const configuration&)
  {   }
  dummy(const std::string&,
        const configuration&,
        const target_type,
        const dependency_set& = dependency_set())
  {   }

  bool add_source_directory(const file_type,
                            const std::string&,
                            file_cache&) override
  { return true; }
  void add_source_file(const file_type,
                       const std::string&,
                       file_cache&,
                       const std::string&,
                       const std::size_t) override
  { return true; }
  void add_library(const std::string&,
                   const std::string&,
                   const std::size_t) override
  { return true; }
};


libambrosia_namespace_end

#endif // AMBROSIA_TARGETS_DUMMY_H
