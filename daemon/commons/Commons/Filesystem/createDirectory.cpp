/*
 * createDirectory.cpp
 *
 */
#include "Commons/Filesystem/createDirectory.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

void createDirectory(const boost::filesystem::path &p)
{
  try
  {
    // sanity check
    const path parent=p.parent_path();
    if( !parent.empty() )
      if( !isDirectorySane(parent) )
        throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "sanity check", "parent's path is not sane");

    // create directory
    if( !create_directory(p) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "create_directory", "unable to create directory");
  }
  catch(const ExceptionFilesystemIO &ex)
  {
    throw ex;
  }
  catch(const std::exception &ex)
  {
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "createDirectory", "problem while creating directory");
  }
}

} // namespace Filesystem
} // namespace Commons
