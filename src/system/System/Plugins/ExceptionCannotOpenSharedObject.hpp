/*
 * ExceptionCannotOpenSharedObject.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_EXCEPTIONCANNOTOPENSHAREDOBJECT_HPP_FILE
#define INCLUDE_SYSTEM_EXCEPTIONCANNOTOPENSHAREDOBJECT_HPP_FILE

/* public header */

#include <boost/filesystem.hpp>

#include "System/Exception.hpp"

namespace System
{
namespace Plugins
{

/** \brief exception thrown when reading symbol failed.
 */
class ExceptionCannotOpenSharedObject: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been raisen.
   *  \param path   path to shared obecjt that accessing failed.
   *  \param reason reason why operation failed.
   */
  ExceptionCannotOpenSharedObject(const Location &where, const boost::filesystem::path &path, const std::string &reason):
    Exception( where, cc("unable to open: '", path.string(), "'; error was: ", reason) )
  {
  }
}; // class ExceptionCannotOpenSharedObject

} // namespace Plugins
} // namespace System

#endif
