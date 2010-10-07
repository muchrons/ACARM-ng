/*
 * isElementSane.cpp
 *
 */
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Logger/Logger.hpp"
#include "Commons/Filesystem/isElementSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isElementSane(const boost::filesystem::path &p)
{
  const Logger::Node log("commons.filesystem.iselementsane");
  LOGMSG_DEBUG_S(log)<<"checking '"<<p<<"' (cannonical: '"<<system_complete(p)<<"')";

  // perform stat
  struct stat st;
  if( stat( p.string().c_str(), &st )!=0 )
  {
    LOGMSG_ERROR_S(log)<<"unable to stat file '"<<p<<"' (connonical: '"<<system_complete(p)<<"')";
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "stat",
                                "unable to stat new file (required to verify file's details)");
  }

  // symlinks are never allowed
  if( symbolic_link_exists(p) )
  {
    LOGMSG_WARN_S(log)<<"file '"<<p<<"' is a symlink - looks suspicious - aborting...";
    return false;
  }

  // symlinks are never allowed
  if( S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) )
  {
    LOGMSG_WARN_S(log)<<"file '"<<p<<"' is a device file - looks suspicious - aborting...";
    return false;
  }

  // check if file has NO hard-links (1==self)
  if( is_regular_file(p) )
  {
    if(st.st_nlink!=1)
    {
      LOGMSG_WARN_S(log)<<"file has "<<st.st_nlink<<" hard-link(s) '"<<p<<"' - looks suspicious - aborting...";
      return false;
    }
  }

  // otherwise it's fine
  LOGMSG_DEBUG_S(log)<<"element '"<<p<<"' (cannonical: '"<<system_complete(p)<<"') looks sane";
  return true;
}

} // namespace Filesystem
} // namespace Commons
