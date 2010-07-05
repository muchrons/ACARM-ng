/*
 * ExceptionConvertionError.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONCONVERTIONERROR_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONCONVERTIONERROR_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thorw on convertion error.
 */
class ExceptionConvertionError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been rised.
   *  \param details details of convertion problem.
   */
  ExceptionConvertionError(const Location &where, const std::string &details):
    Exception(where, cc("convertion failed: ", details) )
  {
  }
}; // class ExceptionConvertionError

} // namespace Persistency

#endif
