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
    /*
       if( !exists(p) )
       throw
       for(boost::filesystem::path::const_iterator it=p.begin();
     */
    // TODO
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
