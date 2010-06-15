/*
 * ThreadIDMap.cpp
 *
 */
#include <algorithm>
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
  const boost::thread::id id=boost::this_thread::get_id();

  Base::Threads::Lock lock(mutex_);

  // has entry in "map"? (NOTE: lower_boundis used here, since it has O(logn) on sorted data.
  IDMap::iterator it=std::lower_bound( m_.begin(), m_.end(), Mapping(id, 0u) );
  if( it!=m_.end() && it->first==id)
  {
    assert( std::find( m_.begin(), m_.end(), Mapping(id, it->second) )==it );
    return it->second;
  }
  // else - add new entry, using lower_bound's result.
  const Mapping tmp(id, nextFreeID_++);
  m_.insert(it, tmp);
  return tmp.second;
}

} // namespace Logger
