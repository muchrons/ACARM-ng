/*
 * ExceptionConversionError.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONCONVERTIONERROR_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONCONVERTIONERROR_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorw on conversion error.
 */
class ExceptionConversionError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been rised.
   *  \param details details of conversion problem.
   */
  ExceptionConversionError(const Location &where, const std::string &details):
    Exception(where, cc("conversion failed: ", details) )
  {
  }
}; // class ExceptionConversionError

} // namespace Persistency

#endif
