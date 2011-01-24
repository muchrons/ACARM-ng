/*
 * HandleSignals.cpp
 *
 */
#include <signal.h>

#include "Core/HandleSignals.hpp"

namespace Core
{

HandleSignals::HandleSignals(WorkThreads *wt):
  sigHUP_(SIGHUP),
  sigINT_ (SIGINT,  wt),
  sigTERM_(SIGTERM, wt),
  sigPIPE_(SIGPIPE)
{
}

} // namespace Core
