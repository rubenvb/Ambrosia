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
 * Ambrosia/Error/error.cpp
 * Class implementation.
 *
 **/

// Class include
#include "Ambrosia/Error/error.h"

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <algorithm>
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

libambrosia_namespace_begin

error::error(const string& message,
             const string_vector& list)
: message(message),
  list(list)
{   }
error::error(const std::string& message,
             const file_set& list)
: message(message),
  list(list.size())
{
  auto dest = std::begin(this->list);
  std::for_each(std::begin(list), std::end(list),
                [=](const file& item) mutable
                { *dest++ = item.name; });
}
error::~error()
{   }

void error::output_message() const
{
  cerr << "Error:\n"
          "\t" << message << "\n";
  if(!list.empty())
  {
    std::for_each(std::begin(list), std::end(list),
                  [](const string& item)
                  { cerr << "\t" << item << "\n";});
  }
}

libambrosia_namespace_end
