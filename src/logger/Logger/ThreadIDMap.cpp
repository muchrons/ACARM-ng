/*
 * ThreadIDMap.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Logger/ThreadIDMap.hpp"

namespace Logger
{

ThreadIDMap::ThreadIDMap(void):
  nextFreeID_(0)
{
}

unsigned int ThreadIDMap::getThreadID(void)
{
  // we can use id_ in thread-safe way without mutexes, since it is
  // thread-local-storage and so is not shared between threads.
  if( id_.get()==NULL )
  {
    // lock is needed here to read/increment nextFreeID_ member, which is shared.
    Base::Threads::Lock lock(mutex_);
    id_.reset( new unsigned int(nextFreeID_++) );
  }
  assert( id_.get()!=NULL );
  return *id_;
}

} // namespace Logger
