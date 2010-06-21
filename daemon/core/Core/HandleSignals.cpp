/*
 * HandleSignals.cpp
 *
 */
#include <signal.h>

#include "Core/HandleSignals.hpp"

namespace Core
{

HandleSignals::HandleSignals(WorkThreads *wt):
  sigINT_ (SIGINT,  wt),
  sigTERM_(SIGTERM, wt),
  sigPIPE_(SIGPIPE, wt)
{
}

} // namespace Core
