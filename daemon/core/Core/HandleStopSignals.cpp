/*
 * HandleStopSignals.cpp
 *
 */
#include <signal.h>

#include "Core/HandleStopSignals.hpp"

namespace Core
{

HandleStopSignals::HandleStopSignals(WorkThreads *wt):
  sigINT_ (SIGINT,  wt),
  sigTERM_(SIGTERM, wt),
  sigPIPE_(SIGPIPE, wt)
{
}

} // namespace Core
