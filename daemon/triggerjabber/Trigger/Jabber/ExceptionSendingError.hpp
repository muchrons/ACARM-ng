/*
 * ExceptionSendingError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_EXCEPTIONSENDINGERROR_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_EXCEPTIONSENDINGERROR_HPP_FILE

#include "Trigger/Jabber/Exception.hpp"

namespace Trigger
{
namespace Jabber
{
/** \brief exception thrown when sending message failed.
 */
class ExceptionSendingError: public Exception
{
public:
  // TODO: 'receive' should be const-ref
  /** \brief create execption with given message.
   *  \param where    place where exception has been thrown.
   *  \param receiver user that was suposed to receive message.
   *  \param ret      error code returned by library
   */
  ExceptionSendingError(const Location &where, const std::string receiver, const int ret):
    Exception(where, cc("unable to send message to user ", receiver, " - error code was ", ret) )
  {
  }
}; // class ExceptionSendingError

} // namespace Jabber
} // namespace Trigger

#endif
