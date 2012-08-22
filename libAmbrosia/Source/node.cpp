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
 * Ambrosia/node.cpp
 * Class implementation.
 *
 **/

// Class/function include
#include "Ambrosia/node.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
#include "Ambrosia/Error/internal_error.h"
#include "Ambrosia/status.h"

// C++ includes
#include <memory>
  using std::shared_ptr;
#include <set>
  using std::set;
#include <string>
  using std::string;

libambrosia_namespace_begin

node::node(const string& name)
: name(name),
  m_edges()
{   }
node::~node()
{   }

void node::add_node(const target& dependency)
{
  if(!m_edges.insert(&dependency).second)
    throw internal_error("Double dependency for node " + name + ": " + dependency.name + ".");
}
const set<const target*> node::edges() const
{
  return m_edges;
}
bool node::operator<(const node& n) const
{
  return name < n.name;
}
bool node::operator==(const node& n) const
{
  return name==n.name;
}

libambrosia_namespace_end
