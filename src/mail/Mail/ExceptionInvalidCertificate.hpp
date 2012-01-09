/*
 * ExceptionInvalidCertificate.hpp
 *
 */
#ifndef INCLUDE_MAIL_EXCEPTIONINVALIDCERTIFICATE_HPP_FILE
#define INCLUDE_MAIL_EXCEPTIONINVALIDCERTIFICATE_HPP_FILE

/* public header */

#include "Mail/ExceptionConnectionError.hpp"

namespace Mail
{
/** \brief exception thrown on invalid certificate (i.e. cert was not verified correctly).
 */
class ExceptionInvalidCertificate: public ExceptionConnectionError
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param server  server that connection was made to.
   *  \param details more detailed problem description.
   */
  ExceptionInvalidCertificate(const Location &where, const char *server, const char *details):
    ExceptionConnectionError(where, server, cc("certificate validation failed: ", details).c_str() )
  {
  }
}; // class ExceptionInvalidCertificate

} // namespace Mail

#endif
