/*
 * ExceptionFilesystemIO.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONFILESYSTEMIO_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONFILESYSTEMIO_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "Commons/Filesystem/Exception.hpp"

namespace Commons
{
namespace Filesystem
{
/** \brief exception thrown on I/O related problems (read/write error, syscalls failure, etc...)
 */
class ExceptionFilesystemIO: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param path    path to file, that has been used.
   *  \param op      operation that failed (ex.: open, close, create, etc...).
   *  \param details some details on error conditions.
   */
  ExceptionFilesystemIO(const Location                &where,
                        const boost::filesystem::path &path,
                        const std::string             &op,
                        const std::string             &details):
    Exception(where, cc("error while performing '", op, "' operation on file '", path, "' (cannonical: '",
                        cc(boost::filesystem::system_complete(path), "'): ", details)) )
  {
  }
}; // class ExceptionFilesystemIO

} // namespace Filesystem
} // namespace Commons

#endif
