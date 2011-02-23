/*
 * ExceptionCannotSetSignalMask.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONCANNOTSETSIGNALMASK_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONCANNOTSETSIGNALMASK_HPP_FILE

/* public header */

#include <string>

#include "System/Exception.hpp"

namespace System
{

/** \brief exception thrown on setting signal mask.
 */
class ExceptionCannotSetSignalMask: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param what  error reason.
   */
  ExceptionCannotSetSignalMask(const Location &where, const std::string &what);
}; // class ExceptionCannotSetSignalMask

} // namespace System

#endif
