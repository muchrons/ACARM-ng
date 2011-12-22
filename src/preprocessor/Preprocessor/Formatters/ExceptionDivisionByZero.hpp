/*
 * ExceptionDivisionByZero.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONDIVISIONBYZERO_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONDIVISIONBYZERO_HPP_FILE

#include "Preprocessor/Formatters/Exception.hpp"


namespace Preprocessor
{
namespace Formatters
{

/** \brief exception thrown when division by zero is requested.
 */
struct ExceptionDivisionByZero: public Exception
{
  /** \brief create error message instance.
   *  \param where location where error was detected.
   */
  explicit ExceptionDivisionByZero(const Location &where);
}; // struct ExceptionDivisionByZero

} // namespace Formatters
} // namespace Preprocessor

#endif
