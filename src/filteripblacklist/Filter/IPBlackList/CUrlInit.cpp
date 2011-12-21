/*
 * CUrlInit.cpp
 *
 */
#include <curlpp/cURLpp.hpp>

#include "Logger/Logger.hpp"
#include "Filter/IPBlackList/CUrlInit.hpp"

namespace Filter
{
namespace IPBlackList
{
namespace detail
{

void CUrlInitImpl::init(void)
{
  Logger::Node log("filter.ipblacklist.curlinit");
  LOGMSG_DEBUG(log, "initializing CURL++ library");
  curlpp::initialize();
  LOGMSG_DEBUG(log, "CURL++ initialized");
}

void CUrlInitImpl::uninit(void)
{
  Logger::Node log("filter.ipblacklist.curlinit");
  LOGMSG_DEBUG(log, "terminating CUrl++ library");
  curlpp::terminate();
  LOGMSG_DEBUG(log, "termination's done");
}

} // namespace detail
} // namespace IPBlackList
} // namespace Filter
