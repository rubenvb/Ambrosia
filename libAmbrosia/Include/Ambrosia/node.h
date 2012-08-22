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
 * Ambrosia/node.h
 * Ambrosia's dependency node for resolving dependencies.
 *
 **/

#ifndef AMBROSIA_NODE_H
#define AMBROSIA_NODE_H

// Global include
#include "Ambrosia/global.h"

// C++ includes
#include <memory>
#include <set>
#include <string>

libambrosia_namespace_begin

// Forward declarations
class target;

class node
{
public:
  node(const std::string& name);
  virtual ~node();

  void add_node(const target& dependency); // sets error status in event of duplicates
  const std::set<const target*> edges() const;
  bool operator<(const node& n) const;
  bool operator==(const node& n) const;

  std::string name;

private:
  std::set<const target*> m_edges; // pointers should not be deleted, as the pointed to objects are owned elsewhere
};

libambrosia_namespace_end

#endif // AMBROSIA_NODE_H
