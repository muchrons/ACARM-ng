/*
 * isDirecotorySane.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Commons/Filesystem/isElementSane.hpp"
#include "Commons/Filesystem/detail/BoostFSCompat.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isDirectorySane(const boost::filesystem::path &p)
{
  try
  {
    // loop through all directories in the path
    path tmp=p;
    do
    {
      if( !isElementSane(tmp) )
        return false;
      if( !is_directory(tmp) )
      {
        const Logger::Node log("commons.filesystem.isdirectorysane");
        LOGMSG_WARN_S(log)<<"element '"<<p<<"' is not directory - aborting...";
        return false;
      }
      // proceed with parent directory
      tmp=detail::parentPath(tmp);
    }
    while( tmp.empty()!=true );

    // if code gets here, it means it's fine
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
