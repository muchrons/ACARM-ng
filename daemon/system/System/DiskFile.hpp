/*
 * DiskFile.hpp
 *
 * representation of an opened file on disk
 * (file is beeing opened during creation).
 *
 */
#ifndef INCLUDE_SYSTEM_DISKFILE_HPP
#define INCLUDE_SYSTEM_DISKFILE_HPP

/* public header */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/AutoDescriptor.hpp"
#include "System/Exception.hpp"


namespace System
{

/** \brief class representing handler to file on disk.
 */
class DiskFile: private boost::noncopyable
{
public:
  /** \brief opens/creates file on disk.
   *  \param fileName name of disk file to be opened.
   *  \param flags    flags to be passed to open() system call.
   *  \param mode     mode to open file.
   */
  DiskFile(const std::string &fileName,
           int    flags=O_RDWR|O_CREAT|O_LARGEFILE,
           mode_t mode =0644);

  /** \brief returns file name.
   *  \return file name.
   */
  inline const std::string &getName(void) const
  {
    return _fileName;
  }
  /** \brief returns file descriptor for given file.
   *  \return file descriptor.
   */
  inline int get(void)
  {
    assert( _fd.isInitialized() );
    return _fd.get();
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
  DiskFile( std::pair<std::string, boost::shared_ptr<AutoDescriptor> > p );

  /** \brief throws error message for given method, making error
   *         information from errno variable.
   */
  void throwFileErrorException(const char *methodName,
                               const char *action);

private:
  const std::string _fileName;
  AutoDescriptor    _fd;
}; // class DiskFile

} // namespace System

#endif

