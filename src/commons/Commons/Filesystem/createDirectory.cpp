/*
 * createDirectory.cpp
 *
 */
#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/createDirectory.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

void createDirectory(const Base::Filesystem::Path &p)
{
  try
  {
    // sanity check
    const path parent=Base::Filesystem::parentPath(p);
    if( !parent.empty() )
      if( !isDirectorySane(parent) )
        throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "sanity check", "parent's path is not sane");

    // create directory
    if( !create_directory(p) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "create_directory", "unable to create directory");
  }
  catch(const ExceptionFilesystemIO &)
  {
    throw;
  }
  catch(const std::exception &ex)
  {
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "createDirectory", ex.what() );
  }
}

} // namespace Filesystem
} // namespace Commons
