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
#include <set>
#include <string>

libambrosia_namespace_begin

class node
{
public:
    node( const std::string &name );
     // can set status::error if there are double dependencies
    node( const std::string &name, const dependency_list &dependencies );
    virtual ~node();

    bool add_node( const node &dependency );
    const std::string &name() const;
    const std::set<node> edges() const;
    bool operator<( const node &n ) const;
    bool operator==( const node &n ) const;

protected:
    std::string m_name;

private:
    std::set<node> m_edges;
};

libambrosia_namespace_end

#endif // NODE_H
