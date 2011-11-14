/*
 * MainImpl.cpp
 *
 */
#include <iostream>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/Exception.hpp"
#include "Core/Main.hpp"
#include "AcarmNG/MainImpl.hpp"
#include "AcarmNG/blockAllSignals.hpp"
#include "AcarmNG/printBanner.hpp"
#include "AcarmNG/randomizeSeed.hpp"

using namespace std;


namespace AcarmNG
{

MainImpl::MainImpl(const int argc, char const * const * const argv):
  log_("MainImpl"),
  clp_(argc, argv),
  appName_(argv[0])
{
  // ensure random number generator is properly seeded
  randomizeSeed();
}


int MainImpl::run(void)
{
  AcarmNG::blockAllSignals();       // permanently disable all signals
  //AcarmNG::printBanner(appName_);    // fancy init :)

  try
  {
    // run main code
    runImpl();
    return 0;
  }
  // TODO: add more catches, with plugin-specific exceptions types
  catch(const Commons::Exception &ex)
  {
    const int ret=16;
    LOGMSG_FATAL_S(log_) << appName_ << ": exception (" << ex.getTypeName()
                         << ") caught: " << ex.what()
                         << "; exiting with code " << ret;
    return ret;
  }
  catch(const std::exception &ex)
  {
    const int ret=8;
    LOGMSG_FATAL_S(log_) << appName_ << ": std::exception caught: "
                         << ex.what() << "; exiting with code " << ret;
    return ret;
  }
  catch(...)
  {
    const int ret=4;
    LOGMSG_FATAL_S(log_) << appName_ << ": unknown exception caught; exiting with code " << ret;
    return ret;
  }

  // code never reaches here
  assert(!"code should never reach here!");
  return 42;
}


void MainImpl::runImpl(void)
{
  LOGMSG_INFO(log_, "starting application");
  Core::Main m;                         // run application (all done in background threads)
  LOGMSG_INFO(log_, "system started");
  m.waitUntilDone();                    // wait until application is done
  LOGMSG_INFO(log_, "system stopped normally");
}

} // namespace AcarmNG
