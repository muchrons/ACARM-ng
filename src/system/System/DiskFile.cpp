/*
 * DiskFile.cpp
 *
 */
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "System/DiskFile.hpp"

using namespace std;
using namespace boost;
namespace fs=boost::filesystem;


namespace System
{

DiskFile::DiskFile(const boost::filesystem::path &fileName, int flags, mode_t mode):
  fileName_(fileName),
  fd_( open( getName().string().c_str(), flags, mode ) )
{
  if( !fd_.isInitialized() )
    throw ExceptionCannotOpenFile(SYSTEM_SAVE_LOCATION, getName() );
}

DiskFile::DiskFile( std::pair<boost::filesystem::path, boost::shared_ptr<AutoDescriptor> > p ):
  fileName_(p.first),
  fd_( *p.second.get() )    // pass the ownership
{
  if( !fd_.isInitialized() )
    throw Exception(SYSTEM_SAVE_LOCATION, "DiskFile::DiskFile(): direct initializing from derived class failed");
}

void DiskFile::unlink(void)
{
  fs::remove( getName() );
}

} // namespace System

