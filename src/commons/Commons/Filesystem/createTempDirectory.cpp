/*
 * createTempDirectory.cpp
 *
 */
#include "System/EditableCString.hpp"
#include "Commons/Filesystem/createTempDirectory.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

Base::Filesystem::Path createTempDirectory(const Base::Filesystem::Path &root)
{
  try
  {
    // sanity check
    if( root.empty() )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "sanity check", "path is not set");
    if( !isDirectorySane(root) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "sanity check", "path is not sane");

    // preapre emplate
    const path              tmpl=root / ".fs_tmp_dir_XXXXXX";
    System::EditableCString tmplStr( tmpl.string() );
    // create directory
    if( mkdtemp( tmplStr.get() )==NULL )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, tmpl, "mkdtemp", "unable to create temporary directory");

    // return new directory
    return path( tmplStr.get() );
  }
  catch(const ExceptionFilesystemIO &)
  {
    throw;
  }
  catch(const std::exception &ex)
  {
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "createTempDirectory", ex.what() );
  }
}

} // namespace Filesystem
} // namespace Commons
