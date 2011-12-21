/*
 * ExceptionUnexpectedFloatingPoint.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONUNEXPECTEDFLOATINGPOINT_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONUNEXPECTEDFLOATINGPOINT_HPP_FILE

/* public header */

#include "Preprocessor/Formatters/Exception.hpp"

namespace Preprocessor
{
namespace Formatters
{

/** \brief exception throw when floating point appears, where non-floating-point number is expected.
 */
class ExceptionUnexpectedFloatingPoint: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param value invalid value.
   */
  ExceptionUnexpectedFloatingPoint(const Location &where, const double value):
    Exception(where, cc("unexpected floating point value: ", value) )
  {
  }
}; // class ExceptionUnexpectedFloatingPoint

} // namespace Formatters
} // namespace Preprocessor

#endif
