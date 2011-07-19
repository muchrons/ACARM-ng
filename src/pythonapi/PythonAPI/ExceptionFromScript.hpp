/*
 * ExceptionFromScript.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_EXCEPTIONFROMSCRIPT_HPP_FILE
#define INCLUDE_PYTHONAPI_EXCEPTIONFROMSCRIPT_HPP_FILE

/* public header */

#include "PythonAPI/Exception.hpp"

namespace PythonAPI
{
/** \brief exception re-thrown when oneir risen from the user's script.
 */
class ExceptionFromScript: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where     place where exception has been thrown.
   *  \param type      type of the exception thrown.
   *  \param msg       message to represent.
   *  \param backtrace stack trace of the call stack.
   */
  ExceptionFromScript(const Location &where, const std::string &type, const std::string &msg, const std::string &backtrace);
}; // class ExceptionFromScript

} // namespace PythonAPI

#endif
