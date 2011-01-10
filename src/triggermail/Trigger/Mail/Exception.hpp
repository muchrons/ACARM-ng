/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_EXCEPTION_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_EXCEPTION_HPP_FILE

#include "Trigger/Exception.hpp"

namespace Trigger
{
namespace Mail
{
/** \brief base for all mail-trigger-related expcetions
 */
class Exception: public Trigger::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  template<typename T>
  Exception(const Location &where, const T &msg):
    Trigger::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace Mail
} // namespace Trigger

#endif
