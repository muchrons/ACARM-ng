/*
 * DiskFile.hpp
 *
 * representation of an opened file on disk
 * (file is beeing opened during creation).
 *
 */
#ifndef INCLUDE_SYSTEM_DISKFILE_HPP_FILE
#define INCLUDE_SYSTEM_DISKFILE_HPP_FILE

/* public header */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/AutoDescriptor.hpp"
#include "System/Exception.hpp"
#include "System/ExceptionSyscallFailed.hpp"

namespace System
{

/** \brief class representing handler to file on disk.
 */
class DiskFile: private boost::noncopyable
{
public:
  /** \brief exception thrown when open() fails to opena file.
   */
  struct ExceptionCannotOpenFile: public ExceptionSyscallFailed
  {
    /** \brief create execption with given message.
     *  \param where place where exception has been raisen.
     *  \param path  file that couldn't be opened.
     */
    ExceptionCannotOpenFile(const Location &where, const boost::filesystem::path &path):
      ExceptionSyscallFailed(where, "open", cc("unable to open file '", path, "'") )
    {
    }
  }; // struct ExceptionCannotOpenFile

  /** \brief opens/creates file on disk.
   *  \param fileName name of disk file to be opened.
   *  \param flags    flags to be passed to open() system call.
   *  \param mode     mode to open file.
   */
  DiskFile(const boost::filesystem::path &fileName,
           int                            flags=O_RDWR|O_CREAT|O_LARGEFILE|O_CLOEXEC,
           mode_t                         mode =0644);

  /** \brief returns file name.
   *  \return file name.
   */
  inline const boost::filesystem::path &getName(void) const
  {
    return fileName_;
  }
  /** \brief returns file descriptor for given file.
   *  \return file descriptor.
   */
  inline int get(void)
  {
    assert( fd_.isInitialized() );
    return fd_.get();
  }

  /** \brief removes file from fielsystem.
   *
   * file is accessible eitherway. it will be phisicaly
   * removed when will not be accessed anymore.
   */
  void unlink(void);

protected:
  /** \brief initializes DiskFile directly from given filename
   *         and its descriptor.
   *
   *  no additional checks are performed. this is useful when
   *  creating unusual constructions like temporary files, etc...
   *
   *  \param p pair <string,descriptor> to be passed for constructor.
   */
  DiskFile( std::pair<boost::filesystem::path, boost::shared_ptr<AutoDescriptor> > p );

private:
  const boost::filesystem::path fileName_;
  AutoDescriptor                fd_;
}; // class DiskFile

} // namespace System

#endif

