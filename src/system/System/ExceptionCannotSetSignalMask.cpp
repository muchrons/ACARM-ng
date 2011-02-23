/*
 * ExceptionCannotSetSignalMask.cpp
 *
 */
#include "System/ExceptionCannotSetSignalMask.hpp"

namespace System
{

ExceptionCannotSetSignalMask::ExceptionCannotSetSignalMask(const Location &where, const std::string &what):
  Exception(where, what)
{
}

} // namespace System
