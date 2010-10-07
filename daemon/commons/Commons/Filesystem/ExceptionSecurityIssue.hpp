/*
 * ExceptionSecurityIssue.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONSECURITYISSUE_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_EXCEPTIONSECURITYISSUE_HPP_FILE

#include <boost/filesystem.hpp>

#include "Commons/Filesystem/Exception.hpp"

namespace Commons
{
namespace Filesystem
{
/** \brief exception thrown when security issue is detected when accessing filesystem.
 */
class ExceptionSecurityIssue: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param path    path to file, that has been used.
   *  \param details details on detected problem / susspicious activity.
   */
  ExceptionSecurityIssue(const Location                &where,
                         const boost::filesystem::path &path,
                         const std::string             &details):
    Exception(where, cc("(potential) security issue detected while operarting on '", path,
                        "' (cannonical: '", boost::filesystem::system_complete(path), "'): ", details) )
  {
  }
}; // class ExceptionFileIO

} // namespace Filesystem
} // namespace Commons

#endif
