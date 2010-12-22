/*
 * ExceptionInvalidDirectory.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_EXCEPTIONINVALIDDIRECTORY_HPP_FILE
#define INCLUDE_PLUGINS_EXCEPTIONINVALIDDIRECTORY_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "Plugins/Exception.hpp"

namespace Plugins
{

/** \brief exception thorw on invalid/non-existing plguins directory.
 */
class ExceptionInvalidDirectory: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param dir   invalid directory path.
   */
  ExceptionInvalidDirectory(const Location &where, const boost::filesystem::path &dir):
    Exception(where, cc("invalid plugins' directory: '", dir, "'") )
  {
  }
}; // class ExceptionInvalidDirectory

} // namespace Plugins

#endif
