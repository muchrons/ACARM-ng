/*
 * ExceptionConnectionError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_EXCEPTIONCONNECTIONERROR_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_EXCEPTIONCONNECTIONERROR_HPP_FILE

#include "Trigger/Mail/Exception.hpp"

namespace Trigger
{
namespace Mail
{
/** \brief exception thrown on connectiong to mail-server error.
 */
class ExceptionConnectionError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  more detailed message.
   */
  ExceptionConnectionError(const Location &where, const char *what):
    Exception(where, cc("connection to e-mail server error: ", what) )
  {
  }
}; // class ExceptionConnectionError

} // namespace Mail
} // namespace Trigger

#endif
