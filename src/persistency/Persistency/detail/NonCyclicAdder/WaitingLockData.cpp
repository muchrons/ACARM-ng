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


WaitingLockData::SignalOnRelease::SignalOnRelease(WaitingLockData &wld):
  wld_(wld)
{
}

WaitingLockData::SignalOnRelease::~SignalOnRelease(void)
{
  try
  {
    wld_.signalAll();   // signal all waiting entities
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
    // wait until something happens.
    // NOTE: timeout is added here, since tryLock() can fail, even if lock is free,
    //       thus making an infinite conditional/wait and effectively hanging program.
    cond_.timed_wait( lock, boost::posix_time::seconds(2) );    // TODO: hardcoded value
  return ptr_;
}

void WaitingLockData::signalAll(void)
{
  cond_.notify_all();
}

void WaitingLockData::setPtrImpl(GraphNodePtr ptr)
{
  ptr_=ptr;
  signalAll();
}

} // namespace detail
} // namespace Persistency
