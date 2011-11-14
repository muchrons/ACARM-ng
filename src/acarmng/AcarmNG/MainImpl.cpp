/*
 * MainImpl.cpp
 *
 */
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <unistd.h>

#include "ConfigConsts/version.hpp"
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

MainImpl::ExceptionCannotDaemonize::ExceptionCannotDaemonize(const Location &where):
  Exception(where, cc("unable to daemonize process: ", strerror(errno)) )
{
}


MainImpl::MainImpl(const int argc, char const * const * const argv):
  log_("mainimpl"),
  clp_(argc, argv),
  appName_(argv[0])
{
  // ensure random number generator is properly seeded
  randomizeSeed();
}


int MainImpl::run(void)
{
  // block all signals, until final handlers will be set
  AcarmNG::blockAllSignals();

  // execture main program code, along with some prelude
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
  // output stream to be used later on
  std::ostream &os=std::cout;

  // see what should be printed
  if( clp_.printHelp() )
    clp_.showHelp(os);
  else
    if( clp_.printVersion() )
      os << ConfigConsts::versionString << endl;
    else
      if( clp_.printBanner() )
        printBanner(os, appName_.c_str() );

  // only printing of some content was to be done?
  if( clp_.quitAfterPrint() )
    return;

  // check if system should be daemonized
  if( clp_.daemonize() )
    runAsDaemon();

  // after all is said and done - run the applicaiton! :)
  runApp();
}


void MainImpl::runApp(void)
{
  LOGMSG_INFO(log_, "starting application");
  Core::Main m;                         // run application (all done in background threads)
  LOGMSG_INFO(log_, "system started");
  m.waitUntilDone();                    // wait until application is done
  LOGMSG_INFO(log_, "system stopped normally");
}


void MainImpl::runAsDaemon(void)
{
  LOGMSG_INFO(log_, "daemonizing process");
  if( daemon(1, 0)!=0 )
  {
    LOGMSG_FATAL_S(log_)<<"daemonizing failed: "<<strerror(errno)<<"; exiting...";
    throw ExceptionCannotDaemonize(SYSTEM_SAVE_LOCATION);
  }
  LOGMSG_INFO(log_, "daemonized process is up and running");
}

} // namespace AcarmNG
