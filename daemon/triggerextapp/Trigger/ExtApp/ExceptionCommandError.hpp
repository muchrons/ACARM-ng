/*
 * ExceptionCommandError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXTAPP_EXCEPTIONCOMMANDERROR_HPP_FILE
#define INCLUDE_TRIGGER_EXTAPP_EXCEPTIONCOMMANDERROR_HPP_FILE

#include "Trigger/ExtApp/Exception.hpp"

namespace Trigger
{
namespace ExtApp
{
/** \brief exception thrown when running command failed.
 */
class ExceptionCommandError: public Exception
{
public:
  /** \brief create exception when running command failed
   *  \param where place where exception has been thrown.
   *  \param cmd   command to be executed.
   */
  ExceptionCommandError(const Location &where, const std::string &cmd):
    Exception(where, cc("unable to run command: '", cmd, "'") )
  {
  }

  /** \brief create execption when command has been run but returned an error.
   *  \param where place where exception has been thrown.
   *  \param cmd   command to be executed.
   *  \param ret   return code returned by the command.
   */
  ExceptionCommandError(const Location &where, const std::string &cmd, int ret):
    Exception(where, cc("command '", cmd, "' returned error code ", ret) )
  {
  }
}; // class Exception

} // namespace ExtApp
} // namespace Trigger

#endif
