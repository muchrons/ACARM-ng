/*
 * ExceptionConnectionError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_EXCEPTIONCONNECTIONERROR_HPP_FILE
#define INCLUDE_TRIGGER_GG_EXCEPTIONCONNECTIONERROR_HPP_FILE

#include "Trigger/GG/Exception.hpp"

namespace Trigger
{
namespace GG
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
    Exception(where, cc("connection to Gadu-Gadu server error: ", what) )
  {
  }
}; // class ExceptionConnectionError

} // namespace GG
} // namespace Trigger

#endif
