/*
 * timestampFromString.cpp
 *
 */
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <cassert>

#include "Persistency/IO/Postgres/timestampFromString.hpp"

using namespace boost::posix_time;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Timestamp timestampFromString(const std::string &str)
{
  const ptime  pt    =time_from_string(str);
  tm           tmTime=to_tm(pt);            // cannot be const, for mktime()
  const time_t tt    =mktime(&tmTime);
  assert(tmTime.tm_isdst==0);
  return Persistency::Timestamp(tt+tmTime.tm_gmtoff);
} // timestampFromString()

} // namespace Postgres
} // namespace IO
} // namespace Persistency
