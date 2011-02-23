/*
 * SignalMaskSetter.cpp
 *
 */
#include "System/detail/SignalMaskSetter.hpp"

namespace System
{
namespace detail
{

SignalMaskSetter::SignalMaskSetter(sigset_t signals)
{
  if( pthread_sigmask(SIG_SETMASK, &signals, &prev_)!=0 )
    throw ExceptionCannotSetSignalMask(SYSTEM_SAVE_LOCATION, "pthread_sigmask(): failed to mask all signals");
}

SignalMaskSetter::~SignalMaskSetter(void)
{
  pthread_sigmask(SIG_SETMASK, &prev_, NULL);
  // NOTE: nothing can be done on error...
}

} // namespace detail
} // namespace System
