/*
 * ExceptionInvalidPrecision.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_EXCEPTIONINVALIDPRECISION_HPP_FILE
#define INCLUDE_SYSTEM_MATH_EXCEPTIONINVALIDPRECISION_HPP_FILE

/* public header */

#include "System/Math/Exception.hpp"

namespace System
{
namespace Math
{

/** \brief exception thrown when invalid precision is required.
 */
class ExceptionInvalidPrecision: public Exception
{
public:
  /** \brief create exception with given message.
   *  \param where place where exception has been raisen.
   *  \param what  name of variable that has been detected NULL.
   *  \param prec  precision that was requested.
   */
  ExceptionInvalidPrecision(const Location &where, const std::string &what, const size_t prec):
    Exception( where, cc("inalivd precision requested: ", prec, " - ", what) )
  {
  }
}; // class ExceptionInvalidPrecision

} // namespace Math
} // namespace System

#endif
