/*
 * ExceptionFileAccessError.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONFILEACCESSERROR_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONFILEACCESSERROR_HPP_FILE

/* public header */

#include <cerrno>
#include <cstring>
#include <boost/filesystem.hpp>

#include "ConfigIO/Exception.hpp"

namespace ConfigIO
{

/** \brief internal exception class.
 */
class ExceptionFileAccessError: public Exception
{
public:
  /** \brief create execption on access error to a file.
   *  \param where place where exception was rised.
   *  \param path  file that caused error.
   */
  ExceptionFileAccessError(const Location &where, const boost::filesystem::path &path):
    Exception(where, cc("unable to access file: '", path, "' - ", strerror(errno) ) )
  {
  }
}; // class ExceptionFileAccessError

} // namespace ConfigIO

#endif
