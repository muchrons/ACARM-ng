/*
 * TimeoutChecker.cpp
 *
 */
#include <tut/tut.hpp>

#include "TestHelpers/TimeoutChecker.hpp"


namespace TestHelpers
{

TimeoutChecker::TimeoutChecker(unsigned int timeout, const char *msg):
  deadline_( time(NULL)+timeout ),
  msg_(msg)
{
}


bool TimeoutChecker::operator()(void) const
{
  if( time(NULL)>deadline_ )
    tut::fail(msg_);
  return true;
}

} // namespace TestHelpers
