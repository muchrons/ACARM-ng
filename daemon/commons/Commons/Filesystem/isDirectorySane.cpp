/*
 * isDirecotorySane.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Commons/Filesystem/isElementSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

bool isDirectorySane(const boost::filesystem::path &p)
{
  try
  {
    path tmp=p;

    // loop through all directories in the path
    do
    {
      if( !isElementSane(tmp) )
        return false;
      if( !is_directory(tmp) )
        return false;
      tmp=tmp.parent_path();
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
