/*
 * ExceptionUnexpectedNegativeNumber.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONUNEXPECTEDNEGATIVENUMBER_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_EXCEPTIONUNEXPECTEDNEGATIVENUMBER_HPP_FILE

/* public header */

#include "Preprocessor/Formatters/Exception.hpp"

namespace Preprocessor
{
namespace Formatters
{

/** \brief exception throw when unexpected negative number is presented.
 */
class ExceptionUnexpectedNegativeNumber: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param value number that was expected to be positive.
   */
  ExceptionUnexpectedNegativeNumber(const Location &where, const double value):
    Exception(where, cc("unexpected negaitve number: ", value) )
  {
  }
}; // class ExceptionUnexpectedNegativeNumber

} // namespace Formatters
} // namespace Preprocessor

#endif
