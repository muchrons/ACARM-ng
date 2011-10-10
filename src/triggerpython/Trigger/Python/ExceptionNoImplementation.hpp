/*
 * ExceptionNoImplementation.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_EXCEPTIONNOIMPLEMENTATION_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_EXCEPTIONNOIMPLEMENTATION_HPP_FILE

#include "Trigger/Python/Exception.hpp"

namespace Trigger
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
  ExceptionNoImplementation(const Location &where, const char *call):
    Exception(where, cc("method ", call, " is not implemented by Python script") )
  {
  }
}; // class ExceptionNoImplementaiton

} // namespace Python
} // namespace Trigger

#endif
