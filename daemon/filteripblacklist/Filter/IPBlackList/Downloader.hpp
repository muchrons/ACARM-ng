/*
 * Downloader.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE

#include <sstream>

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
   *  \note when c-tor is done, data are already downaloded.
   */
  explicit Downloader(const unsigned int limit=10000);
  /** \brief gets string downloaded as response.
   *  \return string with downaloeded data.
   */
  inline std::string get(void) const
  {
    return ss_.str();
  }

private:
  std::stringstream ss_;
}; // class Downloader

} // namespace IPBlackList
} // namespace Filter

#endif
