/*
 * ExceptionInvalidRange.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_EXCEPTIONINVALIDRANGE_HPP_FILE
#define INCLUDE_SYSTEM_MATH_EXCEPTIONINVALIDRANGE_HPP_FILE

/* public header */

#include "System/Math/Exception.hpp"

namespace System
{
namespace Math
{

/** \brief exception thrown when invalid range has been given for operation.
 */
class ExceptionInvalidRange: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param what  name of variable that has been detected NULL.
   */
  ExceptionInvalidRange(const Location &where, const std::string &what):
    Exception( where, cc("inalivd range: ", what) )
  {
  }
}; // class ExceptionInvalidRange

} // namespace Math
} // namespace System

#endif
