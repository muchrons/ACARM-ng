/*
 * WaitingLockData.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/NonCyclicAdder/LockOnWrite_InternalImplementation.hpp"

using namespace Base::Threads;

namespace Persistency
{
namespace detail
{

WaitingLockData::ResetOnRelease::ResetOnRelease(WaitingLockData &wld):
  wld_(wld)
{
}

WaitingLockData::ResetOnRelease::~ResetOnRelease(void)
{
  try
  {
    wld_.setPtr( GraphNodePtr() );  // set to NULL and signal
  }
  catch(...)
  {
    // nothing to be done here
  }
}

WaitingLockData::WaitingLockData(void)
{
  assert( ptr_.get()==NULL );
}

WaitingLockData::~WaitingLockData(void)
{
  try
  {
    // it is safe to do so, in case someone waited on lock
    setPtr( GraphNodePtr() );
  }
  catch(...)
  {
    // nothing can be done here...
  }
}

void WaitingLockData::setPtr(GraphNodePtr ptr)
{
  Lock lock(mutex_);
  setPtrImpl(ptr);
}

GraphNodePtr WaitingLockData::getPtr(void) const
{
  Lock lock(mutex_);
  return ptr_;
}

GraphNodePtr WaitingLockData::getWhenDifferOrLocked(GraphNodePtr                ptr,
                                                    Base::Threads::ReadTryLock &rtl)
{
  Lock lock(mutex_);
  // while pointer has not changed and we're unable to lock try-lock mutex
  // for reading, on a given node, wait for the event.
  while( ptr_==ptr && rtl.tryLock()==false )
    cond_.wait(lock);   // wait until something happens
  return ptr_;
}

void WaitingLockData::setPtrImpl(GraphNodePtr ptr)
{
  ptr_=ptr;
  cond_.notify_all();
}

} // namespace detail
} // namespace Persistency
