/*
 * ExceptionNullParameter.hpp
 *
 */
#ifndef INCLUDE_LOGGER_EXCEPTIONNULLPARAMETER_HPP_FILE
#define INCLUDE_LOGGER_EXCEPTIONNULLPARAMETER_HPP_FILE

/* public header */

#include <cassert>

#include "Logger/Exception.hpp"

namespace Logger
{

/** \brief internal exception class.
 */
class ExceptionNullParameter: public Exception
{
public:
  /** \brief create execption on null parameter
   *  \param where place where exception was rised
   *  \param param parameter name that was null
   */
  explicit ExceptionNullParameter(const char *where, const char *param):
    Exception( std::string( check(where) ) + ": NULL parameter: " + check(param) )
  {
  }

private:
  const char *check(const char *str)
  {
    assert(str!=NULL);
    return str;
  }
}; // class Exception

}; // namespace Logger

#endif
