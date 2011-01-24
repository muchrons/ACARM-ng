/*
 * ExceptionDownloadFailed.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_EXCEPTIONDOWNLOADFAILED_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_EXCEPTIONDOWNLOADFAILED_HPP_FILE

#include "Filter/Exception.hpp"

namespace Filter
{
namespace IPBlackList
{
/** \brief exception on download error
 */
class ExceptionDownloadFailed: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  error details.
   */
  ExceptionDownloadFailed(const Location &where,
                          const char     *what):
    Filter::Exception( where,
                       "ipblacklist",
                       cc("downloading dshield ip bloack list failed: ",
                          what).c_str() )
  {
  }
}; // class ExceptionDownloadFailed

} // namespace IPBlackList
} // namespace Filter

#endif
