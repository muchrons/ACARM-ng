/*
 * ExceptionFileAccessError.hpp
 *
 */
#ifndef INCLUDE_LOGGER_EXCEPTIONFILEACCESSERROR_HPP_FILE
#define INCLUDE_LOGGER_EXCEPTIONFILEACCESSERROR_HPP_FILE

/* public header */

#include <cerrno>
#include <cstring>

#include "Logger/Exception.hpp"

namespace Logger
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
  explicit ExceptionFileAccessError(const Location &where, const char *path):
    Exception(where, std::string(": unable to access file: '") +
                     ensureValidString(path) + "' - " + strerror(errno) )
  {
  }
}; // class Exception

}; // namespace Logger

#endif
