/*
 * ExceptionInvalidConfig.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_EXCEPTIONINVALIDCONFIG_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_EXCEPTIONINVALIDCONFIG_HPP_FILE

#include "Trigger/Mail/Exception.hpp"

namespace Trigger
{
namespace Mail
{
/** \brief exception thrown on invalid configuration parameter
 */
class ExceptionInvalidConfig: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param name    paramter's name.
   *  \param value   invalid value.
   *  \param details additional pieces of information, if available
   */
  ExceptionInvalidConfig(const Location &where,
                         const char     *name,
                         const char     *value,
                         const char     *details=""):
    Exception(where, cc("invalid configuration - paramter '",
                        name, "' has invalid value: '", value, "'; ", details) )
  {
  }
}; // class ExceptionInvalidConfig

} // namespace Mail
} // namespace Trigger

#endif
