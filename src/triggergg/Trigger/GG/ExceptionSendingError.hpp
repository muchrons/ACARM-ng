/*
 * ExceptionSendingError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_EXCEPTIONSENDINGERROR_HPP_FILE
#define INCLUDE_TRIGGER_GG_EXCEPTIONSENDINGERROR_HPP_FILE

#include "Trigger/GG/Exception.hpp"
#include "Trigger/GG/UserID.hpp"

namespace Trigger
{
namespace GG
{
/** \brief exception thrown when sending message failed.
 */
class ExceptionSendingError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where    place where exception has been thrown.
   *  \param receiver user that was suposed to receive message.
   *  \param ret      core returned by library
   */
  ExceptionSendingError(const Location &where, const UserID receiver, const int ret):
    Exception(where, cc("unable to send message to user ", receiver, " - error code was ", ret) )
  {
  }
}; // class ExceptionSendingError

} // namespace GG
} // namespace Trigger

#endif
