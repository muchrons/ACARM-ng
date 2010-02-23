/*
 * GraphNode.cpp
 *
 */
#include <exception>
#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Commons/ViaCollection.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/IO/GlobalConnection.hpp"

namespace Persistency
{

GraphNode::GraphNode(AlertPtrNN           alert,
                     IO::ConnectionPtrNN  connection,
                     IO::Transaction     &t):
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
  nonCyclicAddition(child);
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

AlertPtrNN GraphNode::getAlert(void)
{
  assert( getMetaAlert().get()!=NULL );
  if( !isLeaf() )
    throw ExceptionNotLeaf(SYSTEM_SAVE_LOCATION, getMetaAlert()->getName().get() );

  assert( leaf_.get()!=NULL );
  return leaf_;
}

bool GraphNode::operator==(const GraphNode &other) const
{
  // check if comparing to self
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

  // if nodes itself are identical, ensure all subtrees are identical too
  return Commons::ViaCollection::equal(children_, other.children_);
}

void GraphNode::ensureIsNode(void) const
{
  assert( self_.get()!=NULL );
  if( isLeaf() )
    throw ExceptionNotNode(SYSTEM_SAVE_LOCATION, self_->getName().get() );
}


//
// TODO: this is a TEMPORARY solution - it must be reworked to use some
//       sort of distributed algorithm, not to block whole structure
//       when non-dependent data parts are being operated on.
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_additionMutex);

void GraphNode::nonCyclicAddition(GraphNodePtrNN child)
{
  const GraphNode *childPtr=child.get();
  assert(childPtr!=NULL);

  // only one addition at a time! (TODO: it's too restrictive)
  System::Threads::SafeInitLock lock(g_additionMutex);

  if( this==childPtr           ||   // instant-cycle
      childPtr->hasCycle(this)    ) // is it possible to access self through child
    throw ExceptionCycleDetected(SYSTEM_SAVE_LOCATION,
                                 child->getMetaAlert()->getName().get(),
                                 getMetaAlert()->getName().get() );

  // if there is no cycle, add new child
  children_.push(child);
}

bool GraphNode::hasCycle(const GraphNode *child) const
{
  assert(child!=NULL);

  // for leaf check if current node is not the one to be added
  if( isLeaf() )
    return this==child;

  //
  // TODO: where is better algorithm with do { for(...) } while() loops, to
  //       be used when no global locking is present.
  //       depending on strategy implemented it might be not needed though.
  //
  for(const_iterator it=begin(); it!=end(); ++it)
  {
    const GraphNode *tmp=it->get();
    assert(tmp!=NULL);
    if( tmp==child || tmp->hasCycle(child) )    // ooops - cycle detected...
      return true;
  } // for(children)

  // no cycle detected - seems ok.
  return false;
}

} // namespace Persistency
