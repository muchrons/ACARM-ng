/*
 * SignalUnmasker.hpp
 *
 */
#include "System/SignalUnmasker.hpp"

namespace System
{

namespace
{
sigset_t unmaskAll(void)
{
  sigset_t set;
  sigemptyset(&set);
  return set;
} // unmaskAll()
} // unnamed namespace

SignalUnmasker::SignalUnmasker(void):
  msk_( unmaskAll() )
{
}

} // namespace System
