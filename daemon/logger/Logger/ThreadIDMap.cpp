/*
 * ThreadIDMap.cpp
 *
 */
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
  const boost::thread::id id=boost::this_thread::get_id();

  Base::Threads::Lock lock(mutex_);
  // has entry in "map"?
  for(IDMap::const_iterator it=m_.begin(); it!=m_.end(); ++it)
    if(it->first==id)
      return it->second;
  // if not - make new one!
  const Mapping tmp(id, nextFreeID_++);
  m_.push_back(tmp);
  return tmp.second;
}

} // namespace Logger
