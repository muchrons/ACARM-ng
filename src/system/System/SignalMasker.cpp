/*
 * SignalMasker.hpp
 *
 */
#include "System/SignalMasker.hpp"

namespace System
{

namespace
{
sigset_t maskAll(void)
{
  sigset_t set;
  sigfillset(&set);
  return set;
} // maskAll()
} // unnamed namespace

SignalMasker::SignalMasker(void):
  msk_( maskAll() )
{
}

} // namespace System
