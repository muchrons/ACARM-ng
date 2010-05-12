/*
 * ExceptionInvalidValue.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXCEPTIONINVALIDVALUE_HPP_FILE
#define INCLUDE_TRIGGER_EXCEPTIONINVALIDVALUE_HPP_FILE

/* public header */

#include "Trigger/Exception.hpp"

namespace Trigger
{
/** \brief exception throw when configuration value is invalid.
 */
class ExceptionInvalidValue: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param extra extra pieces of information, if present
   */
  ExceptionInvalidValue(const Location &where,
                        const char     *parameter,
                        const char     *value,
                        const char     *extra=""):
    Exception(where, cc("paramter ", parameter, " has invalid value '", value, "'; details: ", extra) )
  {
  }
}; // class ExceptionInvalidValue

} // namespace Trigger

#endif
