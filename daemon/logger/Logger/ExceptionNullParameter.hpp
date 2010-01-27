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
  explicit ExceptionNullParameter(const Location &where, const char *param):
    Exception(where, std::string(": NULL parameter: ") +
                     ensureValidString(param) )
  {
  }
}; // class Exception

}; // namespace Logger

#endif
