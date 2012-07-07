/**
  * Ambrosia/node.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class/function include
#include "Ambrosia/node.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"
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
: m_name( name ),
  m_edges()
{   }
node::~node()
{   }

void node::add_node(const target& dependency)
{
  if(!m_edges.insert( &dependency ).second)
    emit_error("Double dependency for node " + m_name + ": " + dependency.name() + ".");
}
const std::string& node::name() const
{
  return m_name;
}
const set<const target*> node::edges() const
{
  return m_edges;
}
bool node::operator<(const node& n) const
{
  return m_name < n.name();
}
bool node::operator==(const node& n) const
{
  return m_name==n.name();
}

libambrosia_namespace_end
