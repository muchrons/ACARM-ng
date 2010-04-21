/*
 * ExceptionDeadlockDetected.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONDEADLOCKDETECTED_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONDEADLOCKDETECTED_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorwn when addition to graph caused deadlock condition.
 */
class ExceptionDeadlockDetected: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where      place where exception has been rised
   *  \param parentName name of a paretnt child was being tried to be attached to.
   *  \param childName  name of child that was ot be attached.
   */
  ExceptionDeadlockDetected(const Location &where,
                            const char     *parentName,
                            const char     *childName):
    Exception(where,
              std::string("adding node '") + ensureString(childName) +
              "' as a child of node '"       + ensureString(parentName) +
              "' caused deadlock condition")
  {
  }
}; // class ExceptionDeadlockDetected

} // namespace Persistency

#endif
