/*
 * Sync.cpp
 *
 */
#include <boost/thread/locks.hpp>

#include "System/Threads/Sync.hpp"

namespace System
{
namespace Threads
{

typedef boost::unique_lock<boost::mutex> Lock;

Sync::Sync(void):
  startFlag_(false),
  doneFlag_(true)
{
}

void Sync::start(void)
{
  Lock lock(mutex_);
  startFlag_=true;
  doneFlag_ =false;
  start_.notify_one();
}

void Sync::waitForStart(void)
{
  Lock lock(mutex_);
  while(!startFlag_)
    start_.wait(lock);
}

void Sync::done(void)
{
  Lock lock(mutex_);
  startFlag_=false;
  doneFlag_ =true;
  done_.notify_one();
}

void Sync::waitForDone(void)
{
  Lock lock(mutex_);
  while(!doneFlag_)
    done_.wait(lock);
}

} // namespac Threads
} // namespac System
