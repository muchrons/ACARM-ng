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

#include <string>
#include <cerrno>
#include <cstring>

#include "System/Exceptions/RuntimeError.hpp"

namespace System
{

/** \brief internal exception class.
 */
class ExceptionSyscallFailed: public Exceptions::RuntimeError<ExceptionSyscallFailed>
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param name  name of the system call that failed.
   *  \param msg   message to represent.
   */
  ExceptionSyscallFailed(const Location &where, const char *name, const std::string &msg);
}; // class ExceptionSyscallFailed

} // namespace System

#endif
