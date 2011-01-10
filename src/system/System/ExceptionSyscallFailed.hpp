/*
 * ExceptionSyscallFailed.hpp
 *
 * this is implementation class of base exception for System library and
 * should not be used as a base class outside of it.
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONSYSCALLFAILED_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONSYSCALLFAILED_HPP_FILE

/* public header */

#include "System/Exceptions/RuntimeError.hpp"

namespace System
{

/** \brief internal exception class.
 */
class ExceptionSyscallFailed: public Exceptions::RuntimeError
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param msg   message to represent.
   */
  template<typename T>
  ExceptionSyscallFailed(const Location &where, const T &msg):
    RuntimeError(where, msg)
  {
  }
}; // class ExceptionSyscallFailed

} // namespace System

#endif
