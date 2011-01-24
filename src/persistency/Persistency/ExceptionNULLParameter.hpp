/*
 * ExceptionNULLParamter.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_EXCEPTIONNULLPARAMETER_HPP_FILE
#define INCLUDE_PERSISTENCY_EXCEPTIONNULLPARAMETER_HPP_FILE

/* public header */

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief exception thrown on NULL paramter
 */
class ExceptionNULLParameter: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised
   *  \param name  paramter name.
   */
  ExceptionNULLParameter(const Location &where, const char *name):
    Exception(where, cc("paramter is NULL: ", name) )
  {
  }
}; // class ExceptionNULLParameter

} // namespace Persistency

#endif
