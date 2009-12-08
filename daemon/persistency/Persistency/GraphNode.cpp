/*
 * GraphNode.cpp
 *
 */
#include <cassert>
#include "Persistency/GraphNode.hpp"

namespace Persistency
{

GraphNode::GraphNode(AlertPtr               alert,
                     IO::ConnectionPtr      connection,
                     const IO::Transaction &t):
  self_( new MetaAlert(alert) ),
  leaf_(alert)
{
  assert( leaf_.get()!=NULL );
  assert( self_.get()!=NULL );
  assert( connection.get()!=NULL );

  // save data to DB
  connection->alert(leaf_, t)->save();
  IO::MetaAlertAutoPtr maIO=connection->metaAlert(self_, t);
  maIO->save();
  maIO->associateWithAlert(leaf_);
}

GraphNode::iterator GraphNode::begin(void)
{
  ensureIsNode();
  return children_.begin();
}

GraphNode::iterator GraphNode::end(void)
{
  ensureIsNode();
  return children_.end();
}

GraphNode::const_iterator GraphNode::begin(void) const
{
  ensureIsNode();
  return children_.begin();
}

GraphNode::const_iterator GraphNode::end(void) const
{
  ensureIsNode();
  return children_.end();
}

void GraphNode::addChild(GraphNodePtr child, IO::MetaAlertAutoPtr maIO)
{
  // TODO: ensure there is not cycle here
  assert( maIO.get()!=NULL );
  maIO->addChild( child->get() );
  // if persistency succeded, save in internal collection too
  children_.push(child);
}

bool GraphNode::isLeaf(void) const
{
  if( leaf_.get()!=NULL )
  {
    assert( children_.size()==0 );
    return true;
  }
  return false;
}

bool GraphNode::isNode(void) const
{
  return !isLeaf();
}

MetaAlertPtr GraphNode::get(void)
{
  assert(self_.get()!=NULL);
  return self_;
}

void GraphNode::ensureIsNode(void) const
{
  assert( self_.get()!=NULL );
  if( !isNode() )
  {
    throw ExceptionNotNode(__FILE__, self_->getName().get() );
  }
}

} // namespace Persistency
