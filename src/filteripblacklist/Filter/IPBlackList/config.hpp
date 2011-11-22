/*
 * config.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_CONFIG_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_CONFIG_HPP_FILE

#include <ctime>

namespace Filter
{
namespace IPBlackList
{

/** \brief ammount of time that is reserved in heartbeats for downloading data packages. */
extern const unsigned int timeWindowForDownload;

/** \brief ammount of time to wait for new data, if there is nothing downlaoded yet. */
extern const time_t maxDelayBetweenDownloadRetries;

} // namespace IPBlackList
} // namespace Filter

#endif
