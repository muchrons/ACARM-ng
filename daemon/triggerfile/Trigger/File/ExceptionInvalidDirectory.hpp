/*
 * ExceptionInvalidDirectory.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_EXCEPTIONINVALIDDIRECTORY_HPP_FILE
#define INCLUDE_TRIGGER_FILE_EXCEPTIONINVALIDDIRECTORY_HPP_FILE

#include "Trigger/File/Exception.hpp"

namespace Trigger
{
namespace File
{
/** \brief exception thrown on invalid directory
 */
class ExceptionInvalidDirectory: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param dir   path to directory.
   */
  ExceptionInvalidDirectory(const Location &where, const std::string &dir):
    Exception(where, cc("invalid or unwritable directory '", dir, "'") )
  {
  }
}; // class Exception

} // namespace File
} // namespace Trigger

#endif
