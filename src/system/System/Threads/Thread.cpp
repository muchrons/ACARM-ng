/*
 * Thread.cpp
 *
 */
#include "System/Threads/Thread.hpp"

namespace System
{
namespace Threads
{

Thread::~Thread(void)
{
  try
  {
    interrupt();
    join();
  }
  catch(...)
  {
    // nothing we can do here...
  }
}

void Thread::join(void)
{
  th_.join();
}

void Thread::interrupt(void)
{
  th_.interrupt();
}

Thread::ID Thread::id(void) const
{
  return th_.get_id();
}

} // namespace Threads
} // namespace System
