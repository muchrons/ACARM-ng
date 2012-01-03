/*
 * ExceptionInvalidFile.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_WHOIS_EXCEPTIONINVALIDFILE_HPP_FILE
#define INCLUDE_TRIGGER_WHOIS_EXCEPTIONINVALIDFILE_HPP_FILE

#include <boost/filesystem.hpp>

#include "Trigger/Whois/Exception.hpp"

namespace Trigger
{
namespace Whois
{
/** \brief exception thrown on invalid file
 */
class ExceptionInvalidFile: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param file  path to file.
   */
  ExceptionInvalidFile(const Location &where, const boost::filesystem::path &file):
    Exception(where, cc("invalid or unreadable file '", file, "'") )
  {
  }
}; // class Exception

} // namespace Whois
} // namespace Trigger

#endif
