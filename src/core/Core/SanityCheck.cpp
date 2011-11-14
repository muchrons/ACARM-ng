/*
 * SanityCheck.cpp
 *
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include "ConfigConsts/version.hpp"
#include "Base/StrError.hpp"
#include "Logger/Logger.hpp"
#include "Core/SanityCheck.hpp"

namespace Core
{

SanityCheck::SanityCheck(void):
  log_("core.sanitycheck")
{
  LOGMSG_DEBUG(log_, "running sanity check...");
  logSystemInfo();
  ensureNotRoot();
  LOGMSG_INFO(log_, "sanity check passed");
}

void SanityCheck::logSystemInfo(void)
{
  LOGMSG_DEBUG(log_, "trying to log basic system info");
  struct utsname buf;
  const int      ret=uname(&buf);
  if(ret!=0)
  {
    // this error is not fatal - we may proceed
    LOGMSG_WARN_S(log_)<<"error returned while calling uname(); error code was: "
                       <<ret<<": "<<Base::StrError().get();
    return;
  }

  LOGMSG_INFO_S(log_)<<"system version '" << ConfigConsts::versionString << "' "
                     <<"running on OS '"  << buf.sysname                 << "' "
                     <<"version '"        << buf.version                 << "' "
                     <<"release '"        << buf.release                 << "' "
                     <<"machine '"        << buf.nodename                << "'";
}

void SanityCheck::ensureNotRoot(void)
{
  LOGMSG_DEBUG(log_, "checking if system's not running as root (UID!=0)");
  if( getuid()==0 )
    throw ExceptionRunningAsRoot(SYSTEM_SAVE_LOCATION);
  LOGMSG_DEBUG_S(log_)<<"system running as UID=="<<getuid();
}

} // namespace Core
