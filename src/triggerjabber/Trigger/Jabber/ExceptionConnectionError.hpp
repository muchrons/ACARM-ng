/*
 * ExceptionConnectionError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_EXCEPTIONCONNECTIONERROR_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_EXCEPTIONCONNECTIONERROR_HPP_FILE

#include "Trigger/Jabber/Exception.hpp"

namespace Trigger
{
namespace Jabber
{
/** \brief exception thrown on communication error with gg-server.
 */
class ExceptionConnectionError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  more detailed message.
   */
  ExceptionConnectionError(const Location &where, const char *what):
    Exception(where, cc("connection to Jabber server error: ", what) )
  {
  }
}; // class ExceptionConnectionError

} // namespace Jabber
} // namespace Trigger

#endif
