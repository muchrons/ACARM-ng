/*
 * ExceptionNoSuchParamter.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONNOSUCHPARAMTER_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONNOSUCHPARAMTER_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{

/** \brief internal exception class.
 */
class ExceptionNoSuchParameter: public ExceptionParseError
{
public:
  /** \brief create execption on invalid parameter request.
   *  \param where place where exception was rised.
   *  \param param parameter name that was null.
   */
  ExceptionNoSuchParameter(const Location &where, const std::string &param):
    ExceptionParseError(where, cc("no such paramter parameter: ", param) )
  {
  }
}; // class ExceptionNoSuchParameter

} // namespace ConfigIO

#endif
