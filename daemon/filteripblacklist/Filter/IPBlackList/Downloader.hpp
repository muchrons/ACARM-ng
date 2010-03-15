/*
 * Downloader.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_DOWNLOADER_HPP_FILE

#include <sstream>

#include "Filter/IPBlackList/ExceptionDownloadFiled.hpp"

namespace Filter
{
namespace IPBlackList
{

/** \brief downloader for dshield black list.
 */
class Downloader
{
public:
  explicit Downloader(const unsigned int limit=10000);

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
