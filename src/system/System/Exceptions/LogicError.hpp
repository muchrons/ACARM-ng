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
protected:
  /** \brief create execption with given message.
   *  \param msg message to represent.
   */
  template<typename T>
  explicit LogicError(const T &msg):
    Exceptions::BaseSimple<LogicError<CRTP>, std::logic_error>(msg)
  {
  }
}; // class LogicError

} // namespace Exceptions
} // namespace System

#endif
