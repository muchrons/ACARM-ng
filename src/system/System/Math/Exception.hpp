/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_EXCEPTIONM_HPP_FILE
#define INCLUDE_SYSTEM_MATH_EXCEPTIONM_HPP_FILE

/* public header */

#include <string>

#include "System/Exception.hpp"

namespace System
{
namespace Math
{

/** \brief generic exception thrown on math issues.
 */
class Exception: public System::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been raisen.
   *  \param what  name of variable that has been detected NULL.
   */
  Exception(const Location &where, const std::string &what):
    System::Exception( where, cc("math error: ", what) )
  {
  }
}; // class Exception

} // namespace Math
} // namespace System

#endif
