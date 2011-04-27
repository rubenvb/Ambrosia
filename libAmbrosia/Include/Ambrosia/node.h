/**
  * Ambrosia/node.h
  * Ambrosia's dependency node for resolving dependencies.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef NODE_H
#define NODE_H

// Global include
#include "global.h"
/* "typedefs.h" */

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
    node( const std::string &name );
    virtual ~node();

    void add_node( const target &dependency ); // sets error status in event of duplicates
    const std::string &name() const;
    const std::set<const target*> edges() const;
    bool operator<( const node &n ) const;
    bool operator==( const node &n ) const;

protected:
    std::string m_name;

private:
    std::set<const target*> m_edges; // pointers should not be deleted, as the pointed to objects are owned elsewhere
};

libambrosia_namespace_end

#endif // NODE_H
