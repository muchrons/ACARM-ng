/*
 * Simple.cpp
 *
 */
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/isChildUnique.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"
#include "Persistency/detail/InternalAccessProxy.hpp"

namespace Persistency
{
namespace detail
{

/** \brief helper object for implementation.
 */
class NonCyclicAdder::InternalImplementation
{
}; // class NonCyclicAdder


NonCyclicAdder::NonCyclicAdder(void):
  data_(NULL)   // internal data is NOT needed here
{
}

NonCyclicAdder::~NonCyclicAdder(void)
{
  assert( data_.get()==NULL );
  // NOTE: required for API compatibility
}


// unnamed namespace for helper calls
namespace
{

bool hasCycle(const GraphNode *from, const GraphNode *to)
{
  assert(from!=NULL);
  assert(to  !=NULL);

  // for leaf check if current node is not the one to be added
  if( from->isLeaf() )
    return from==to;

  // search for cycle in each child
  for(GraphNode::const_iterator it=from->begin(); it!=from->end(); ++it)
  {
    const GraphNode *tmp=it->get();
    assert(tmp!=NULL);
    if( tmp==to || hasCycle(tmp, to) )  // ooops - cycle detected...
      return true;
  } // for(children)

  // no cycle detected - seems ok.
  return false;
} // hasCycle()


// initilialize safe init lock (on compile time)
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_additionMutex);

} // unnamed namespace

bool NonCyclicAdder::addChildImpl(GraphNode           &parent,
                                  InternalAccessProxy &iap,
                                  GraphNodePtrNN       child)
{
  assert( data_.get()==NULL );
  const GraphNode *childPtr =child.get();
  const GraphNode *parentPtr=&parent;
  assert(childPtr !=NULL);
  assert(parentPtr!=NULL);

  // only one addition at a time!
  System::Threads::SafeInitLock lock(g_additionMutex);

  // skip addition if child's alreadypresent
  if( !isChildUnique(parent, child) )
    return false;

  // check for cycle
  if( parentPtr==childPtr           ||      // instant-cycle?
      hasCycle(childPtr, parentPtr)    )    // is it possible to access self through child?
    throw ExceptionCycleDetected( SYSTEM_SAVE_LOCATION,
                                  parentPtr->getMetaAlert()->getName().get(),
                                  childPtr->getMetaAlert()->getName().get() );

  // if there is no cycle, add new child
  iap.addChildToChildrenVector(parent, child);
  return true;
}

} // namespace detail
} // namespace Persistency
