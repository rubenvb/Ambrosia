/**
  * Ambrosia/node.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class/function include
#include "node.h"

// libAmbrosia includes
#include "algorithm.h"
#include "status.h"

// C++ includes
/* <set> */
    using std::set;
/* <string> */
    using std::string;

libambrosia_namespace_begin

node::node( const string &name )
:   m_name( name ),
    m_edges()
{   }
node::node( const string &name, const dependency_list &dependencies )
:   m_name( name ),
    m_edges()
{
    const auto end = dependencies.end();
    for( auto it = dependencies.begin(); it != end; ++it )
    {
        const string name( (*it).second );
        if( !add_node(name) )
            emit_error( "Double dependency: " + name + " for target: " + m_name + "." );
    }
}
node::~node()
{   }

bool node::add_node( const node &dependency )
{
    return m_edges.insert( dependency ).second;
}
const std::string & node::name() const
{
    return m_name;
}
const set<node> node::edges() const
{
    return m_edges;
}
bool node::operator<( const node &n ) const
{
    return m_name < n.name();
}
bool node::operator==( const node &n ) const
{
    return m_name==n.name();
}

libambrosia_namespace_end
