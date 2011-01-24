/*
 * ExceptionCannotReadSymbol.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONCANNOTREADSYMBOL_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONCANNOTREADSYMBOL_HPP_FILE

/* public header */

#include "System/Exception.hpp"

namespace System
{
namespace Plugins
{

/** \brief exception thrown when reading symbol failed.
 */
class ExceptionCannotReadSymbol: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been raisen.
   *  \param name   name of the non-existing symbol.
   *  \param reason details on error.
   */
  ExceptionCannotReadSymbol(const Location &where, const std::string &name, const std::string &reason):
    Exception( where, cc("given symbol name is not valid: '", name, "'; error was: ", reason) )
  {
  }
}; // class ExceptionCannotReadSymbol

} // namespace Plugins
} // namespace System

#endif
