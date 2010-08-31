/*
 * ExceptionCannotOpenFile.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_EXCEPTIONCANNOTOPENFILE_HPP_FILE
#define INCLUDE_TRIGGER_FILE_EXCEPTIONCANNOTOPENFILE_HPP_FILE

#include "Trigger/File/Exception.hpp"

namespace Trigger
{
namespace File
{
/** \brief exception thrown when output file cannot be opened.
 */
class ExceptionCannotOpenFile: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param path    path to file, that has been used.
   *  \param details some details on error conditions.
   */
  ExceptionCannotOpenFile(const Location &where, const std::string &path, const std::string &details):
    Exception(where, cc("error while opening/creating file for writing: '", path, "' - ", details) )
  {
  }
}; // class ExceptionCannotOpenFile

} // namespace File
} // namespace Trigger

#endif
