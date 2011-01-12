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


namespace System
{

DiskFile::DiskFile(const std::string &fileName, int flags, mode_t mode):
  fileName_(fileName),
  fd_( open( fileName_.c_str(), flags, mode ) )
{
  if( !fd_.isInitialized() )
    throwFileErrorException("DiskFile::DiskFile()", "open");
}

DiskFile::DiskFile( std::pair<std::string, boost::shared_ptr<AutoDescriptor> > p ):
  fileName_(p.first),
  fd_( *p.second.get() )    // pass the ownership
{
  if( !fd_.isInitialized() )
    throw Exception(SYSTEM_SAVE_LOCATION, "DiskFile::DiskFile(): direct initializing from derived class failed");
}


void DiskFile::throwFileErrorException(const char *methodName,
                                       const char *action)
{
  stringstream ss;
  ss<<methodName<<": unable to "<<action<<" '"
    <<getName()<<"' file: "<< strerror(errno);
  // TODO: make this separate exception type
  throw Exception( SYSTEM_SAVE_LOCATION, ss.str() );
}


void DiskFile::unlink(void)
{
  if( ::unlink( getName().c_str() )!=0 )
    if(errno!=ENOENT)   // if file was already removed - ok
      throwFileErrorException("DiskFile::unlink()", "unlink");
}

} // namespace System

