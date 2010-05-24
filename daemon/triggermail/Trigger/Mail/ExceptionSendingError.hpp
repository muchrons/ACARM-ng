/*
 * ExceptionSendingError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_EXCEPTIONSENDINGERROR_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_EXCEPTIONSENDINGERROR_HPP_FILE

#include "Trigger/Mail/Exception.hpp"
#include "Trigger/Mail/UserID.hpp"

namespace Trigger
{
namespace Mail
{
/** \brief exception thrown when sending message failed.
 */
class ExceptionSendingError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where    place where exception has been thrown.
   *  \param sender   mail of the sending account.
   *  \param receiver mail of the receiver.
   */
  ExceptionSendingError(const Location &where, const char *sender, const char *receiver):
    Exception(where, cc("unable to send message to '",
                        receiver, "' from '", sender, "': error is: ") )
  {
  }
}; // class ExceptionConnectionError

} // namespace Mail
} // namespace Trigger

#endif
