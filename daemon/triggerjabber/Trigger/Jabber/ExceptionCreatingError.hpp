/*
 * ExceptionSendingError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_EXCEPTIONCREATINGERROR_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_EXCEPTIONCREATINGERROR_HPP_FILE

#include "Trigger/Jabber/Exception.hpp"

namespace Trigger
{
namespace Jabber
{
/** \brief exception thrown when sending message failed.
 */
class ExceptionCreatingError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where    place where exception has been thrown.
   *  \param receiver user that was suposed to receive message.
   *  \param ret      error code returned by library
   */
  ExceptionCreatingError(const Location &where, const std::string &receiver):
    Exception(where, cc("unable to create message to user ", receiver) )
  {
  }
}; // class ExceptionCreatingError

} // namespace Jabber
} // namespace Trigger

#endif
