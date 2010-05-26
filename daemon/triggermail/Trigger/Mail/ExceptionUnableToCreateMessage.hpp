/*
 * ExceptionUnableToCreateMessage.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_EXCEPTIONUNABLETOCREATEMESSAGE_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_EXCEPTIONUNABLETOCREATEMESSAGE_HPP_FILE

#include "Trigger/Mail/Exception.hpp"

namespace Trigger
{
namespace Mail
{
/** \brief exception thrown on problems with creating message.
 */
class ExceptionUnableToCreateMessage: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  reason of error.
   */
  ExceptionUnableToCreateMessage(const Location &where,
                                 const char     *what):
    Exception(where, cc("problem creating message: ", what) )
  {
  }
}; // class ExceptionUnableToCreateMessage

} // namespace Mail
} // namespace Trigger

#endif
