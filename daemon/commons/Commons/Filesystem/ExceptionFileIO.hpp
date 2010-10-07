/*
 * ExceptionFileIO.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONFILEIO_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONFILEIO_HPP_FILE

#include "Commons/Filesystem/Exception.hpp"

namespace Commons
{
namespace Filesystem
{
/** \brief exception thrown when output file cannot be opened.
 */
class ExceptionFileIO: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param path    path to file, that has been used.
   *  \param op      operation that failed (ex.: open, close, create, etc...).
   *  \param details some details on error conditions.
   */
  ExceptionFileIO(const Location &where, const std::string &path, const std::string &op, const std::string &details):
    Exception(where, cc("error while performing '", op, "' operation on file '", path, "': ", details) )
  {
  }
}; // class ExceptionFileIO

} // namespace Filesystem
} // namespace Commons

#endif
