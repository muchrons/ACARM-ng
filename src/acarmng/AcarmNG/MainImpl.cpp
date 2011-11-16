/*
 * MainImpl.cpp
 *
 */
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>

#include "ConfigConsts/version.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Exception.hpp"
#include "Core/Main.hpp"
#include "AcarmNG/MainImpl.hpp"
#include "AcarmNG/printBanner.hpp"
#include "AcarmNG/CmdLineParser.hpp"
#include "AcarmNG/randomizeSeed.hpp"
#include "AcarmNG/blockAllSignals.hpp"

using namespace std;


namespace AcarmNG
{

MainImpl::ExceptionCannotDaemonize::ExceptionCannotDaemonize(const Location &where):
  Exception(where, cc("unable to daemonize process: ", strerror(errno)) )
{
}


MainImpl::ExceptionCannotDropPrivileges::ExceptionCannotDropPrivileges(const Location &where, const char *type, const int from, const int to):
  Exception(where, cc("unable to drop privileges (", type, ") from ", from, " to ", to))
{
}


MainImpl::MainImpl(const int argc, char const * const * const argv):
  log_("acarmng.mainimpl"),
  cla_(argc, argv),
  appName_( cla_.argv()[0] )
{
  assert(appName_!=NULL);
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
  catch(const CmdLineParser::ExceptionParameterError &ex)
  {
    // show help screen in case of parameter error
    const int ret=32;
    LOGMSG_FATAL_S(log_) << appName_ << ": exception (" << ex.getTypeName()
                         << ") caught: " << ex.what()
                         << "; exiting with code " << ret;
    cerr << appName_ << ": " << ex.what() << endl;
    CmdLineParser::showHelp(cerr);
    return ret;
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
  // parse command line
  const CmdLineParser clp( cla_.argc(), cla_.argv() );
  // do not work as root
  dropPrivileges( clp.userID(), clp.groupID() );
  // output stream to be used later on
  ostream &os=cout;

  // see what should be printed
  if( clp.printHelp() )
    clp.showHelp(os);
  else
    if( clp.printVersion() )
      os << ConfigConsts::versionString << endl;
    else
      if( clp.printBanner() )
        printBanner(os, appName_);

  // only printing of some content was to be done?
  if( clp.quitAfterPrint() )
    return;

  // check if system should be daemonized
  if( clp.daemonize() )
    runAsDaemon();

  // after all is said and done - run the applicaiton! :)
  runApp();
}


void MainImpl::dropPrivileges(const uid_t uid, const gid_t gid)
{
  // if needed, first drop group ID
  if( getgid()!=gid )
  {
    LOGMSG_INFO_S(log_)<<"dropping GID from "<<getgid()<<" to "<<gid;
    if( setgid(gid)!=0 )
      throw ExceptionCannotDropPrivileges(SYSTEM_SAVE_LOCATION, "GID", getgid(), gid);
  }
  else
    LOGMSG_DEBUG_S(log_)<<"GID is already "<<getgid()<<", as reuqired";

  // if needed, drop user ID
  if( getuid()!=uid )
  {
    LOGMSG_INFO_S(log_)<<"dropping UID from "<<getuid()<<" to "<<uid;
    if( setuid(uid)!=0 )
      throw ExceptionCannotDropPrivileges(SYSTEM_SAVE_LOCATION, "UID", getuid(), uid);
  }
  else
    LOGMSG_DEBUG_S(log_)<<"UID is already "<<getuid()<<", as reuqired";

  // summary
  LOGMSG_INFO_S(log_)<<"system now uses privileges of UID="<<getuid()<<" / GID="<<getgid();
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


void MainImpl::runApp(void)
{
  LOGMSG_INFO(log_, "starting application");
  Core::Main m;                         // run application (all done in background threads)
  LOGMSG_INFO(log_, "system started");
  m.waitUntilDone();                    // wait until application is done
  LOGMSG_INFO(log_, "system stopped normally");
}

} // namespace AcarmNG
