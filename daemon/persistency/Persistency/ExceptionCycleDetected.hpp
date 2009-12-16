/*
 * ExceptionCycleDetected.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONCYCLEDETECTED_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONCYCLEDETECTED_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorw when addition of a given element to a given node
 *         would have create a cycle in graph.
 */
class ExceptionCycleDetected: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where      place where exception has been rised
   *  \param parentName name of a paretnt child was being tried to be attached to.
   *  \param childName  name of child that was ot be attached.
   */
  ExceptionCycleDetected(const char *where,
                         const char *parentName,
                         const char *childName):
    Exception(where,
              std::string("addition node '") + ensureString(childName) +
              "' as a child of node '"       + ensureString(parentName) +
              "' would cause a cycle")
  {
  }
}; // class ExceptionCycleDetected

}; // namespace Persistency

#endif
