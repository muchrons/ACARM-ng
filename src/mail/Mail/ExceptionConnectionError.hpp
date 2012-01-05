/*
 * ExceptionConnectionError.hpp
 *
 */
#ifndef INCLUDE_MAIL_EXCEPTIONCONNECTIONERROR_HPP_FILE
#define INCLUDE_MAIL_EXCEPTIONCONNECTIONERROR_HPP_FILE

#include "Mail/Exception.hpp"

namespace Mail
{
/** \brief exception thrown on connectiong to mail-server error.
 */
class ExceptionConnectionError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been thrown.
   *  \param server server address, connecting to failed.
   *  \param reason problem description.
   */
  ExceptionConnectionError(const Location &where, const char *server, const char *reason):
    Exception(where, cc("connection to e-mail server '", server, "' error: ", reason) )
  {
  }
}; // class ExceptionConnectionError

} // namespace Mail

#endif
