/*
 * openElement.cpp
 *
 */
#include <cassert>

#include "Commons/Filesystem/openElement.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Commons/Filesystem/isFifoSane.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> openElement(const boost::filesystem::path &p,
                                           const Mode                     mode,
                                           bool (*isSane)(const boost::filesystem::path &) )
{
  assert(isSane!=NULL);
  // only given element type can be opened
  if( !(*isSane)(p) )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "isSane", "path looks suspicious - refusing to open");

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
