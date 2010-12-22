/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_EXCEPTION_HPP_FILE
#define INCLUDE_PLUGINS_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Plugins
{

/** \brief base exception for plugins-related issues.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  Exception(const Location &where, const std::string &msg):
    Commons::Exception(where, msg)
  {
  }
}; // class Exception

} // namespace Plugins

#endif
