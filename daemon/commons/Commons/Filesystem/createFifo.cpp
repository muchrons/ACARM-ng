/*
 * createFifo.cpp
 *
 */
#include <sys/types.h>
#include <sys/stat.h>

#include "Commons/Filesystem/createFifo.hpp"
#include "Commons/Filesystem/openFifo.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Commons/Filesystem/detail/BoostFSCompat.hpp"

using namespace boost::filesystem;

namespace Commons
{
namespace Filesystem
{

SharedPtrNotNULL<std::fstream> createFifo(const boost::filesystem::path &p)
{
  // fifo cannot exist yet
  if( exists(p) )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "exists", "element already exists");

  // directory path must be sane
  const path parent=detail::parentPath(p);
  if( parent.empty()==false )
  {
    if( isDirectorySane(parent)==false )
      throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "isDirectorySane",
                                  "path looks suspicious - refusing to create fifo");
  }

  // create new fifo
  if( mkfifo( p.string().c_str(), 0644 )!=0 )
    throw ExceptionFilesystemIO(SYSTEM_SAVE_LOCATION, p, "mkfifo", "unable to create fifo queue");

  // opens fifo
  return openFifo(p);
}

} // namespace Filesystem
} // namespace Commons
