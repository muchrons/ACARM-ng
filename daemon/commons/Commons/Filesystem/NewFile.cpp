/*
 * NewFile.cpp
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "Logger/Logger.hpp"
#include "Commons/Filesystem/NewFile.hpp"

namespace Commons
{
namespace Filesystem
{

NewFile::NewFile(const std::string &path):
  log_("trigger.file.ensurefile"),
  fd_( open( path.c_str(), O_CREAT|O_EXCL, 0600 ) )
{
  // check if file has been opened
  if(fd_.get()==-1)
  {
    LOGMSG_ERROR_S(log_)<<"unable to create/open file '"<<path<<"'";
    throw ExceptionFileIO(SYSTEM_SAVE_LOCATION, path, "create", "creating file failed");
  }

  // run stat in order to perform some extra checks
  struct stat st;
  if( fstat( get(), &st )==-1 )
  {
    LOGMSG_ERROR_S(log_)<<"unable to state file '"<<path<<"'";
    throw ExceptionFileIO(SYSTEM_SAVE_LOCATION, path, "stat", "unable to stat new file (required to verify file's details)");
  }
  // check if file's size is 0
  if(st.st_size!=0)
  {
    LOGMSG_ERROR_S(log_)<<"file's size is non-zero '"<<path<<"'";
    throw ExceptionFileIO(SYSTEM_SAVE_LOCATION, path, "-", "file's size is non-zero");
  }
  // check if file has NO hard-links (1==self)
  if(st.st_nlink!=1)
  {
    LOGMSG_ERROR_S(log_)<<"file has hard-link(s) '"<<path<<"' - looks suspicious - aborting...";
    throw ExceptionFileIO(SYSTEM_SAVE_LOCATION, path, "-", "file has hard-link(s) to it");
  }

  LOGMSG_DEBUG_S(log_)<<"new file created successfully: '"<<path<<"'";
}

} // namespace Filesystem
} // namespace Commons
