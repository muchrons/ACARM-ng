/*
 * createTempFile.cpp
 *
 */
#include <cstdlib>
#include <unistd.h>

#include "System/EditableCString.hpp"
#include "Commons/Filesystem/createTempFile.hpp"
#include "Commons/Filesystem/openFile.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace Base::Filesystem;

namespace Commons
{
namespace Filesystem
{

TempFile createTempFile(const Base::Filesystem::Path &root)
{
  // directory must already exist
  if( !exists(root) )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "exists", "root directory does not exist");

  // directory path must be sane
  if( isDirectorySane(root)==false )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, root, "isDirectorySane",
                                "path looks suspicious - refusing to create file");

  // prepare template
  const Base::Filesystem::Path tmpl=root / ".fs_tmp_file_XXXXXX";
  System::EditableCString      tmplStr( tmpl.string() );
  // create file
  const int fd=mkstemp( tmplStr.get() );
  if(fd==-1)
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, tmpl, "mkstemp", "unable to create temporary file");
  close(fd);        // close valid descriptor, if opened

  // return final object
  const Path file( tmplStr.get() );
  return TempFile( openFile(file), file );
}

} // namespace Filesystem
} // namespace Commons
