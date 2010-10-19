/*
 * isFifoSane.cpp
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/Filesystem/isFifoSane.hpp"
#include "Commons/Filesystem/isElementSane.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isFifoSane(const boost::filesystem::path &p)
{
  try
  {
    const Logger::Node log("commons.filesystem.isfifosane");

    // loop through all directories in the path
    if( !isElementSane(p) )
      return false;
    if( is_regular_file(p) )
    {
      LOGMSG_WARN_S(log)<<"element '"<<p<<"' is regular file - aborting...";
      return false;
    }
    if( is_directory(p) )
    {
      LOGMSG_WARN_S(log)<<"element '"<<p<<"' is directory - aborting...";
      return false;
    }

    // test parent directories, if specified
    const path parent=p.parent_path();
    if( parent.empty()==false )
      return isDirectorySane(parent);

    // perform lstat (stat LINK, not place pointed to!)
    struct stat st;
    if( lstat( p.string().c_str(), &st )!=0 )
    {
      LOGMSG_ERROR_S(log)<<"unable to stat fifo '"<<p<<"' (connonical: '"<<system_complete(p)<<"')";
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "lstat",
          "unable to stat fifo (required to verify fifo's details)");
    }

    // only fifo is allowed here
    if( !S_ISFIFO(st.st_mode) )
    {
      LOGMSG_WARN_S(log)<<"element '"<<p<<"' is not a fifo (named pipe) - looks suspicious - aborting...";
      return false;
    }

    // if code gets here, it means it's fine
    LOGMSG_DEBUG_S(log)<<"element '"<<p<<"' looks like sane fifo queue";
    return true;
  }
  catch(const filesystem_error &ex)
  {
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "?", ex.what() );
  }
  assert(!"code ever reaches here");
}

} // namespace Filesystem
} // namespace Commons
