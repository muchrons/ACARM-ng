/*
 * ExceptionCantOpenFile.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_EXCEPTIONCANTOPENFILE_HPP_FILE
#define INCLUDE_TRIGGER_FILE_EXCEPTIONCANTOPENFILE_HPP_FILE

#include "Trigger/File/Exception.hpp"

namespace Trigger
{
namespace File
{
/** \brief exception thrown when output file cannot be opened.
 */
class ExceptionCantOpenFile: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param path  path to file, that has been used.
   */
  ExceptionCantOpenFile(const Location &where, const std::string &path):
    Exception(where, cc("unable to open file (for writing): '", path, "'") )
  {
  }
}; // class Exception

} // namespace File
} // namespace Trigger

#endif
