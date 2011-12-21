/*
 * ExceptionNotANumber.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXCEPTIONNOTANUMBER_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXCEPTIONNOTANUMBER_HPP_FILE

/* public header */

#include "Preprocessor/Exception.hpp"

namespace Preprocessor
{
namespace Formatters
{

/** \brief exception throw when a non-number value was passed and a number was expected.
 */
class ExceptionNotANumber: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param str   value that was expected to be a number.
   */
  ExceptionNotANumber(const Location    &where,
                      const std::string &str):
    Exception(where, cc("not-a-number passed; got '", str, "' while number was expected") )
  {
  }
}; // class ExceptionNotANumber

} // namespace Formatters
} // namespace Preprocessor

#endif
