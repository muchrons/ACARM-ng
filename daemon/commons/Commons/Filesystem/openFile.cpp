/*
 * openFile.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/openFile.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openFile(const boost::filesystem::path &p, const Mode mode)
{
  // file cannot exist yet
  if( !exists(p) )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "exists", "file does not exist");

  // directory path must be sane
  const path parent=p.parent_path();
  if( !parent.empty() )
  {
    if( !isDirectorySane(parent) )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "isDirectorySane",
                                  "path looks suspicious - refusing to create file");
  }

  // determine opening mode
  std::fstream::openmode openMode=std::fstream::binary;
  if( mode.toInt() & Mode::READ )
    openMode|=std::fstream::in;
  if( mode.toInt() & Mode::WRITE )
    openMode|=std::fstream::out;
  // open file
  SharedPtrNotNULL<std::fstream> out( new std::fstream(p.string().c_str(), openMode) );
  assert( out.get()!=NULL );
  if( out->is_open()==false )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "fstream/open", "unable to open file");
  // return final object
  return out;
}

} // namespace Filesystem
} // namespace Commons
