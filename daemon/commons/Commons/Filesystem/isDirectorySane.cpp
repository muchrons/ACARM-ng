/*
 * isDirecotorySane.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isDirectorySane(const boost::filesystem::path &p)
{
  try
  {
    if( !exists(p) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "exist", "directory does not exist");
    if( !is_directory(p) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "is_directory", "not a directory");
    // now check each element
    for(boost::filesystem::path::const_iterator it=p.begin(); it!=p.end(); ++it)
    {
      // TODO
    }
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
