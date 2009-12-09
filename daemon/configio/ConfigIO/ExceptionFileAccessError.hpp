/*
 * ExceptionFileAccessError.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONFILEACCESSERROR_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONFILEACCESSERROR_HPP_FILE

/* public header */

#include <cerrno>
#include <cstring>

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
  explicit ExceptionFileAccessError(const char *where, const char *path):
    Exception( std::string( ensureValidString(where) ) + ": unable to access file: '" +
               ensureValidString(path) + "' - " + strerror(errno) )
  {
  }
}; // class Exception

}; // namespace ConfigIO

#endif
