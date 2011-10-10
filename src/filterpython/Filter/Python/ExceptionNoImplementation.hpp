/*
 * ExceptionNoImplementation.hpp
 *
 */
#ifndef INCLUDE_FILTER_PYTHON_EXCEPTIONNOIMPLEMENTATION_HPP_FILE
#define INCLUDE_FILTER_PYTHON_EXCEPTIONNOIMPLEMENTATION_HPP_FILE

#include "Filter/Python/Exception.hpp"

namespace Filter
{
namespace Python
{

/** \brief exceptionthrow when implementation cannot be found.
 */
class ExceptionNoImplementation: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param call  method that is not implementd, but should be.
   */
  ExceptionNoImplementation(const Location &where, const char *call);
}; // class ExceptionNoImplementaiton

} // namespace Python
} // namespace Filter

#endif
