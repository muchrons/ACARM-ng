/*
 * ExceptionNullParameter.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONNULLPARAMETER_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONNULLPARAMETER_HPP_FILE

/* public header */

#include <cassert>

#include "ConfigIO/Exception.hpp"

namespace ConfigIO
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
    Exception( std::string( ensureValidString(where) ) + ": NULL parameter: " +
               ensureValidString(param) )
  {
  }
}; // class Exception

}; // namespace ConfigIO

#endif
