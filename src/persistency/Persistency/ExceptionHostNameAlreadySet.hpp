/*
 * ExceptionHostNameAlreadySaved.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONHOSTNAMEALREADYSET_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONHOSTNAMEALREADYSET_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{
/** \brief exception throw when trying to set again name for a given host.
 */
class ExceptionHostNameAlreadySet: public Exception
{
public:
  /** \brief c-tor with proper error message.
   *  \param where place where exception has been raised.
   *  \param name  host's name that is alreayd set.
   */
  ExceptionHostNameAlreadySet(const Location &where, const char *name);
}; // class ExceptionHostNameAlreadySet

} // namespace Persistency

#endif
