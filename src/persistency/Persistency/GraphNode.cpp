/*
 * GraphNode.cpp
 *
 */
#include <exception>
#include <cassert>

#include "Commons/ViaUnorderedSortableCollection.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/InternalAccessProxy.hpp"
#include "Persistency/IO/GlobalConnection.hpp"

namespace Persistency
{

GraphNode::GraphNode(AlertPtrNN           alert,
                     MetaAlert::ID        id,
                     IO::ConnectionPtrNN  connection,
                     IO::Transaction     &t):
  self_( new MetaAlert(alert, id) ),
  leaf_( alert.shared_ptr() )
{
  assert( leaf_.get()!=NULL );
  assert( self_.get()!=NULL );
  assert( connection.get()!=NULL );

  // save data to DB
  connection->alert(leaf_, t)->save();
  IO::MetaAlertAutoPtr maIO=connection->metaAlert( getMetaAlert(), t);
  maIO->save();
  maIO->associateWithAlert(leaf_);
  maIO->markAsUsed();

  assert( isLeaf() && "invalid initialization");
}

GraphNode::GraphNode(MetaAlertPtrNN            ma,
                     IO::ConnectionPtrNN       connection,
                     IO::Transaction          &t,
                     const NodeChildrenVector &children):
  self_(ma),
  leaf_()
{
  assert( leaf_.get()==NULL );
  assert( self_.get()!=NULL );
  assert( connection.get()!=NULL );
  assert( children.size()>=2 );

  // save data to DB along with adding elements to graph
  IO::MetaAlertAutoPtr maIO=connection->metaAlert( getMetaAlert(), t);
  assert( maIO.get()!=NULL );
  maIO->save();
  for(NodeChildrenVector::const_iterator it=children.begin(); it!=children.end(); ++it)
    addChild(*it, *maIO);
  maIO->markAsUsed();

  assert( !isLeaf() && "invalid initialization");
}

GraphNode::~GraphNode(void)
{
  try
  {
    IO::GlobalConnection::get()->markAsUnused( getMetaAlert() );
  }
  catch(const std::exception &ex)
  {
    Logger::Node log("persistency.graphnode");
    LOGMSG_ERROR_S(log)<<"unable to mark meta alert as unused: "<<ex.what();
  }
  catch(...)
  {
    Logger::Node log("persistency.graphnode");
    LOGMSG_ERROR(log, "unable to mark meta alert as unused (unknown exception)");
  }
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

void GraphNode::addChild(GraphNodePtrNN child, IO::MetaAlert &maIO)
{
  ensureIsNode();
  // check if addition will not cause cycle
  detail::InternalAccessProxy iap;
  // if node has not been added (i.e.: is already present as a child), just
  // skip this call and do not try to save anything in data base.
  if( !nca_.addChildImpl(*this, iap, child) )
    return;
  // persistency save
  maIO.addChild( child->getMetaAlert() );
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

MetaAlertPtrNN GraphNode::getMetaAlert(void)
{
  assert(self_.get()!=NULL);
  return self_;
}

ConstMetaAlertPtrNN GraphNode::getMetaAlert(void) const
{
  assert(self_.get()!=NULL);
  return self_;
}

AlertPtrNN GraphNode::getAlert(void)
{
  assert( getMetaAlert().get()!=NULL );
  if( !isLeaf() )
    throw ExceptionNotLeaf(SYSTEM_SAVE_LOCATION, getMetaAlert()->getName().get() );

  assert( leaf_.get()!=NULL );
  return leaf_;
}

ConstAlertPtrNN GraphNode::getAlert(void) const
{
  if( !isLeaf() )
    throw ExceptionNotLeaf(SYSTEM_SAVE_LOCATION, getMetaAlert()->getName().get() );

  assert( leaf_.get()!=NULL );
  return leaf_;
}

namespace
{
struct GraphNodeSWO
{
  bool operator()(const ConstGraphNodePtrNN &n1, const ConstGraphNodePtrNN &n2) const
  {
    return n1->getMetaAlert()->getID() < n2->getMetaAlert()->getID();
  }
}; // struct GraphNodeSWO
} // unnamed namespace

bool GraphNode::operator==(const GraphNode &other) const
{
  // check comparing to self
  if(this==&other)
    return true;
  assert( self_.get()!=other.self_.get() );

  // compare content
  if( isLeaf()!=other.isLeaf() )
    return false;
  if( *self_!=*other.self_ )
    return false;
  if( isLeaf() )    // if one is leaf, second is leaf too - checked before
  {
    assert( leaf_.get()!=other.leaf_.get() );
    if( *leaf_!=*other.leaf_ )
      return false;
  }

  // check children, but skip the order, since it may differ in general case
  return Commons::ViaUnorderedSortableCollection::equal( children_, other.children_, (GraphNodeSWO()) );
}

void GraphNode::ensureIsNode(void) const
{
  assert( self_.get()!=NULL );
  if( isLeaf() )
    throw ExceptionNotNode(SYSTEM_SAVE_LOCATION, self_->getName().get() );
}

} // namespace Persistency
