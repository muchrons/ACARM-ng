/*
 * ExceptionSyscallFailed.cpp
 *
 */
#include <cerrno>
#include <cstring>

#include "System/ExceptionSyscallFailed.hpp"

namespace System
{

ExceptionSyscallFailed::ExceptionSyscallFailed(const Location &where, const char *name, const std::string &msg):
  Exceptions::RuntimeError<ExceptionSyscallFailed>(where, cc("syscall '", name, "' failed - ", msg, ": ", strerror(errno)) )
{
}

} // namespace System
