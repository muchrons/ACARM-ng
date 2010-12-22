/*
 * ExceptionInvalidPlugin.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_EXCEPTIONINVALIDPLUGIN_HPP_FILE
#define INCLUDE_PLUGINS_EXCEPTIONINVALIDPLUGIN_HPP_FILE

/* public header */

#include "Plugins/Exception.hpp"

namespace Plugins
{

/** \brief exception thorw on invalid/non-existing plugin.
 */
class ExceptionInvalidPlugin: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   error message (details).
   */
  ExceptionInvalidPlugin(const Location &where, const std::string &msg):
    Exception(where, cc("invalid plugin; error is: ", msg) )
  {
  }
}; // class ExceptionInvalidPlugin

} // namespace Plugins

#endif
