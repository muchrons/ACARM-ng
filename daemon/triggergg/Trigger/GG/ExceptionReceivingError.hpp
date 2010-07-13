/*
 * ExceptionReceivingError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_EXCEPTIONRECEIVINGERROR_HPP_FILE
#define INCLUDE_TRIGGER_GG_EXCEPTIONRECEIVINGERROR_HPP_FILE

#include "Trigger/GG/Exception.hpp"

namespace Trigger
{
namespace GG
{
/** \brief exception thrown when reveiving message failed.
 */
class ExceptionReceivingError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been thrown.
   *  \param action action that failed.
   */
  ExceptionReceivingError(const Location &where, const std::string &action):
    Exception(where, cc("unable to receive message: ", action) )
  {
  }
}; // class ExceptionReceivingError

} // namespace GG
} // namespace Trigger

#endif
