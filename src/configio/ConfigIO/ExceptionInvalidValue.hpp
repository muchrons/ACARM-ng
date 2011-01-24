/*
 * ExceptionInvalidValue.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_EXCEPTIONINVALIDVALUE_HPP_FILE
#define INCLUDE_CONFIGIO_EXCEPTIONINVALIDVALUE_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/Exception.hpp"

namespace ConfigIO
{

/** \brief exception thrown on invalid value given to config.
 */
class ExceptionInvalidValue: public Exception
{
public:
  /** \brief create execption on parse error.
   *  \param where  place where exception was rised.
   *  \param param  paramter name, that has incorrect value.
   *  \param value  value that was not accepted.
   *  \param reason reason while given value was not accepted.
   */
  template<typename T>
  ExceptionInvalidValue(const Location    &where,
                        const std::string &param,
                        const T           &value,
                        const std::string &reason):
    Exception(where, cc("invalid config value '", value, "' for paramter '",
                        param, "': ", reason) )
  {
  }
}; // class ExceptionInvalidValue

} // namespace ConfigIO

#endif
