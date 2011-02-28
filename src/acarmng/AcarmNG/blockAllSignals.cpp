/*
 * main.cpp
 *
 */
#include <cstring>
#include <signal.h>

#include "AcarmNG/blockAllSignals.hpp"

extern "C"
{
static void ignoreSignal(int /*s*/)
{
  // do noting
} // ignoreSignal()
} // extern "C"


namespace AcarmNG
{

void blockAllSignals(void)
{
  // prepare data
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa) );
  sa.sa_handler=ignoreSignal;
  // register as a handle for all signals
  for(int i=1; i<=64; ++i)
    sigaction(i, &sa, NULL);    // will fail for some signals - never mind that
} // blockAllSignals()

} // namespace AcarmNG
