/*
 * SafeInitLocking.cpp
 *
 */
#include <iostream>
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"

using namespace std;

namespace System
{
namespace Threads
{

//
// NOTE: c-tor/d-tor only shows messages since in geneal it is a bad idea to
//       close application from library level. though IF it will ever happen
//       that any of these conditions will fail, application my become
//       useless (hang, crash or simillar)...
//

SafeInitLock::SafeInitLock(pthread_mutex_t &mutex):
  mutex_(mutex)
{
  if( pthread_mutex_lock(&mutex_)!=0 )
  {
    assert(!"SafeInitLock::SafeInitLock(): unable to lock mutex");
    cerr<<"SafeInitLock::SafeInitLock(): unable to lock mutex - "
          "critical section IS NOT PROTECTED!"<<endl;
  }
}

SafeInitLock::~SafeInitLock(void)
{
  if( pthread_mutex_unlock(&mutex_)!=0 )
  {
    assert(!"SafeInitLock::~SafeInitLock(): unable to unlock mutex");
    cerr<<"SafeInitLock::~SafeInitLock(): unable to unlock mutex - "
          "critical section IS NOT RELEASED!"<<endl;
  }
}

} // namespace Threads
} // namespace System
