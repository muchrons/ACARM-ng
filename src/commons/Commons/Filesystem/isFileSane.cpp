/*
 * isFileSane.cpp
 *
 */
#include <cassert>

#include "Base/Filesystem/BoostFS.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Commons/Filesystem/isElementSane.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isFileSane(const Base::Filesystem::Path &p)
{
  try
  {
    // loop through all directories in the path
    if( !isElementSane(p) )
      return false;
    if( !Base::Filesystem::isRegularFile(p) )
    {
      const Logger::Node log("commons.filesystem.isfilesane");
      LOGMSG_WARN_S(log)<<"element '"<<p<<"' is not regular file - aborting...";
      return false;
    }

    // test parent directories, if specified
    const Base::Filesystem::Path parent=Base::Filesystem::parentPath(p);
    if( parent.empty()==false )
      return isDirectorySane(parent);

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
