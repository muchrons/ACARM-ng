/*
 * ExceptionPointerIsNULL.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONPOINTERISNULL_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONPOINTERISNULL_HPP_FILE

/* public header */

#include "System/Exception.hpp"

namespace System
{

/** \brief exception when NULL pointer is passed, when not expected.
 */
class ExceptionPointerIsNULL: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param name  name of variable that has been detected NULL.
   */
  ExceptionPointerIsNULL(const Location &where, const char *name):
    Exception( where, cc("NULL pointer recieved: ", name) )
  {
  }
}; // class ExceptionPointerIsNULL

} // namespace System

#endif
