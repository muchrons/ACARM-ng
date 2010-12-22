/*
 * ExceptionRegistrationError.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_EXCEPTIONREGISTRATIONERROR_HPP_FILE
#define INCLUDE_PLUGINS_EXCEPTIONREGISTRATIONERROR_HPP_FILE

/* public header */

#include "Plugins/Exception.hpp"

namespace Plugins
{

/** \brief exception thrown on registration error.
 */
class ExceptionRegistrationError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of the factory that could not be registered.
   *  \param msg   message to represent.
   */
  ExceptionRegistrationError(const Location &where, const std::string &name, const std::string &msg):
    Exception(where, cc("unable to register factory '", name, "': ", msg) )
  {
  }
}; // class ExceptionRegistrationError

} // namespace Plugins

#endif
