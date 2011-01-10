/*
 * Downloader.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE

#include <string>

#include "Filter/IPBlackList/CUrlInit.hpp"
#include "Filter/IPBlackList/ExceptionDownloadFailed.hpp"

namespace Filter
{
namespace IPBlackList
{

/** \brief downloader for dshield black list.
 */
class Downloader
{
public:
  /** \brief create object with downloaded data.
   *  \param limit limit number of entries.
   */
  explicit Downloader(const unsigned int limit=10000);
  /** \brief gets string downloaded as response.
   *  \return string with downaloeded data.
   */
  std::string download(void) const;

private:
  const CUrlInit    curlInit_;
  const std::string url_;
}; // class Downloader

} // namespace IPBlackList
} // namespace Filter

#endif
