/*
 * LogicError.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONS_LOGICERROR_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONS_LOGICERROR_HPP_FILE

/* public header */

#include "System/Exceptions/BaseSimple.hpp"

namespace System
{
namespace Exceptions
{

/** \brief base exception class for errors in program logic.
 *  \note throwing this type of error may be threated as an release-time
 *        assert equivalent.
 */
template<typename CRTP>
class LogicError: public BaseSimple<LogicError<CRTP>, std::logic_error>
{
private:
  typedef BaseSimple<LogicError<CRTP>, std::logic_error> BaseC;

protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit LogicError(const T &msg):
    BaseC(msg)
  {
  }
  /** \brief create execption with given message.
   *  \param where location of exception raising.
   *  \param msg   message to represent.
   */
  template<typename T>
  LogicError(const typename BaseC::Location &where, const T &msg):
    BaseC(where, msg)
  {
  }
}; // class LogicError

} // namespace Exceptions
} // namespace System

#endif
