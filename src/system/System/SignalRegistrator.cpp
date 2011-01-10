/*
 * SignalRegistrator.cpp
 *
 */
#include <cassert>
#include <cstring>

#include "System/SignalRegistrator.hpp"

namespace System
{

SignalRegistrator::~SignalRegistrator(void)
{
  // return to oryginal handler
  if( sigaction(signum_, &previous_, NULL)!=0 )
    assert(!"unable to unregister signal handler and set previous one");
}

SignalRegistrator::SignalRegistrator(int signum, void (*handler)(int) ):
  signum_(signum)
{
  // check input
  if(handler==NULL)
    throw ExceptionPointerIsNULL(SYSTEM_SAVE_LOCATION, "handler");

  // prepare data structures
  memset(&current_, 0, sizeof(current_) );
  current_.sa_handler=handler;
  // register handler
  if( sigaction(signum_, &current_, &previous_)!=0 )
    throw ExceptionRegistrationFailed(SYSTEM_SAVE_LOCATION);
}

} // namespace System
