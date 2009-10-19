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
  _fileName(fileName),
  _fd( open( _fileName.c_str(), flags, mode ) )
{
  if( !_fd.isInitialized() )
    throwFileErrorException("DiskFile::DiskFile()", "open");
}

DiskFile::DiskFile( std::pair<std::string, boost::shared_ptr<AutoDescriptor> > p ):
  _fileName(p.first),
  _fd( *p.second.get() )    // pass the ownership
{
  if( !_fd.isInitialized() )
    throw Exception("DiskFile::DiskFile(): "
                    "direct initializing from derived class failed");
}


void DiskFile::throwFileErrorException(const char *methodName,
                                       const char *action)
{
  stringstream ss;
  ss<<methodName<<": unable to "<<action<<" '"
    <<getName()<<"' file: "<< strerror(errno);
  throw Exception( ss.str() );
}


void DiskFile::unlink(void)
{
  if( ::unlink( getName().c_str() )!=0 )
    if(errno!=ENOENT)   // if file was already removed - ok
      throwFileErrorException("DiskFile::unlink()", "unlink");
}

} // namespace System

