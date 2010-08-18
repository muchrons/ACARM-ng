/*
 * GraphNode.cpp
 *
 */
#include <exception>
#include <cassert>

#include "Commons/ViaCollection.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/InternalAccessProxy.hpp"
#include "Persistency/IO/GlobalConnection.hpp"

namespace Persistency
{

GraphNode::IDAssigner::~IDAssigner(void)
{
}


namespace
{
GraphNode::ID getAssignedID(IO::ConnectionPtrNN    connection,
                            IO::Transaction       &t,
                            GraphNode::IDAssigner &idAssigner)
{
  IO::DynamicConfigAutoPtr dc=connection->dynamicConfig("persistency", t);
  assert( dc.get()!=NULL );
  return idAssigner.assign(*dc);
} // getAssignedID()
} // unnamed namespace


GraphNode::GraphNode(AlertPtrNN           alert,
                     IO::ConnectionPtrNN  connection,
                     IO::Transaction     &t,
                     IDAssigner          &idAssigner):
  id_( getAssignedID(connection, t, idAssigner) ),
  self_( new MetaAlert(alert) ),
  leaf_(alert)
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
                     const NodeChildrenVector &children,
                     IDAssigner               &idAssigner):
  id_( getAssignedID(connection, t, idAssigner) ),
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
    LOGMSG_ERROR_S(log)<<"unambe to mark meta alert as unused: "<<ex.what();
  }
  catch(...)
  {
    Logger::Node log("persistency.graphnode");
    LOGMSG_ERROR(log, "unambe to mark meta alert as unused (unknown exception)");
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

GraphNode::ID GraphNode::getID(void) const
{
  return id_;
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

AlertPtrNN GraphNode::getAlert(void)
{
  assert( getMetaAlert().get()!=NULL );
  if( !isLeaf() )
    throw ExceptionNotLeaf(SYSTEM_SAVE_LOCATION, getMetaAlert()->getName().get() );

  assert( leaf_.get()!=NULL );
  return leaf_;
}

const MetaAlert &GraphNode::getMetaAlert(void) const
{
  assert(self_.get()!=NULL);
  return *self_;
}

const Alert &GraphNode::getAlert(void) const
{
  if( !isLeaf() )
    throw ExceptionNotLeaf(SYSTEM_SAVE_LOCATION, getMetaAlert().getName().get() );

  assert( leaf_.get()!=NULL );
  return *leaf_;
}

bool GraphNode::operator==(const GraphNode &other) const
{
  // check if comparing to self
  if(this==&other)
    return true;
  assert( self_.get()!=other.self_.get() );

  // compare IDs
  if( getID()!=other.getID() )
    return false;

  // TODO: following content is in fact redundant and probably should be replaced with asserts
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

  // if nodes itself are identical, ensure all subtrees are identical too
  return Commons::ViaCollection::equal(children_, other.children_);
}

void GraphNode::ensureIsNode(void) const
{
  assert( self_.get()!=NULL );
  if( isLeaf() )
    throw ExceptionNotNode(SYSTEM_SAVE_LOCATION, self_->getName().get() );
}

} // namespace Persistency
