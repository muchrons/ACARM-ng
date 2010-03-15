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
  ExceptionFileAccessError(const Location &where, const char *path):
    Exception(where, std::string("unable to access file: '") +
                     ensureValidString(path) + "' - " + strerror(errno) )
  {
  }
}; // class ExceptionFileAccessError

} // namespace ConfigIO

#endif
