/*
 * createTempFile.cpp
 *
 */
#include <cstdlib>
#include <unistd.h>

#include "Base/EditableCString.hpp"
#include "Commons/Filesystem/createTempFile.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

TempFile createTempFile(const boost::filesystem::path &root)
{
  // directory must already exist
  if( !exists(root) )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "exists", "root directory does not exist");

  // directory path must be sane
  if( isDirectorySane(root)==false )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "isDirectorySane",
                                "path looks suspicious - refusing to create file");

  // prepare template
  const path            tmpl=root / ".fs_tmp_file_XXXXXX";
  Base::EditableCString tmplStr( tmpl.string() );
  // create file
  const int fd=mkstemp( tmplStr.get() );
  if(fd==-1)
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, tmpl, "mkstemp", "unable to create temporary file");
  close(fd);        // close valid descriptor, if opened

  // return final object
  const path                      path( tmplStr.get() );
  boost::shared_ptr<std::fstream> sharedFile( openFile(path).release() );
  return TempFile(sharedFile, path);
}

} // namespace Filesystem
} // namespace Commons
