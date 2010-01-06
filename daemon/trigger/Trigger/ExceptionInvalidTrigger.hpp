/*
 * ExceptionInvalidTrigger.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXCEPTIONINVALIDTRIGGER_HPP_FILE
#define INCLUDE_TRIGGER_EXCEPTIONINVALIDTRIGGER_HPP_FILE

/* public header */

#include <string>

#include "Trigger/Exception.hpp"

namespace Trigger
{
/** \brief exception thrown when invalid trigger is used.
 */
class ExceptionInvalidTrigger: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of invalid trigger.
   */
  ExceptionInvalidTrigger(const char *where, const char *name):
    Exception(where, std::string("invalid trigger: ")+ensureString(name) )
  {
  }
}; // class ExceptionInvalidTrigger

} // namespace Filter

#endif
