/*
 * ExceptionParseError.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONPARSEERROR_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONPARSEERROR_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/Exception.hpp"

namespace ConfigIO
{

/** \brief generic parsing error exception.
 */
class ExceptionParseError: public Exception
{
public:
  /** \brief create execption on parse error.
   *  \param where place where exception was rised.
   *  \param what  problem description.
   */
  ExceptionParseError(const Location &where, const std::string &what):
    Exception(where, cc("config parse error: ", what) )
  {
  }
}; // class ExceptionParseError

} // namespace ConfigIO

#endif
