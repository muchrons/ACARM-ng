/*
 * ExceptionInvalidExpression.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXCEPTIONINVALIDEXPRESSION_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXCEPTIONINVALIDEXPRESSION_HPP_FILE

/* public header */

#include "Preprocessor/Exception.hpp"

namespace Preprocessor
{
/** \brief exception throw when incorrect expression has been detected
 */
class ExceptionInvalidExpression: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  reason for this error.
   */
  ExceptionInvalidExpression(const Location    &where,
                             const std::string &what):
    Exception(where, what)
  {
  }
}; // class ExceptionInvalidExpression

} // namespace Preprocessor

#endif
