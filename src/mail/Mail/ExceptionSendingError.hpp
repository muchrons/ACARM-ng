/*
 * ExceptionSendingError.hpp
 *
 */
#ifndef INCLUDE_MAIL_EXCEPTIONSENDINGERROR_HPP_FILE
#define INCLUDE_MAIL_EXCEPTIONSENDINGERROR_HPP_FILE

#include "Mail/Exception.hpp"

namespace Mail
{
/** \brief exception thrown when sending message failed.
 */
class ExceptionSendingError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where     place where exception has been thrown.
   *  \param sender    mail of the sending account.
   *  \param receivers mail of the receiver.
   *  \param error     error message.
   */
  ExceptionSendingError(const Location &where,
                        const char     *sender,
                        const char     *receivers,
                        const char     *error):
    Exception(where, cc("unable to send message to '",
                        receivers, "' from '", sender, "': error is: ", error) )
  {
  }
}; // class ExceptionConnectionError

} // namespace Mail

#endif
