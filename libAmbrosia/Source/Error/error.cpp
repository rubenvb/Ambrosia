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
: m_message(message),
  m_list(list)
{   }
error::error(const std::string& message,
             const file_set& list)
: m_message(message),
  m_list(list.size())
{
  auto dest = m_list.begin();
  std::for_each(list.begin(), list.end(), [=](const file& item) mutable
                                          { *dest++ = item.first; });
}
error::~error()
{   }

void error::output_message() const
{
  cerr << "Error:\n"
          "\t" << m_message << "\n";
  if(!m_list.empty())
  {
    std::for_each(m_list.begin(), m_list.end(),
                  [](const string& item)
                  { cerr << "\t" << item << "\n";});
  }
}

libambrosia_namespace_end
