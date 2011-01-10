/*
 * Timestamp.cpp
 *
 */
#include <ctime>
#include <cstdio>
#include <cassert>

#include "Persistency/Timestamp.hpp"

namespace Persistency
{

Timestamp::Timestamp(void):
  ts_( time(NULL) )
{
}

Timestamp::Timestamp(time_t ts):
  ts_(ts)
{
}

std::string Timestamp::str(void) const
{
  const time_t tt=get();
  struct tm    bt;
  if( gmtime_r(&tt, &bt)!=&bt )
    throw ExceptionConversionError(SYSTEM_SAVE_LOCATION, "gmtime_r() returned invalid pointer");

  char tmp[5+3+2 +1 +3+3+2 +1];
  if( snprintf(tmp, sizeof(tmp), "%04d.%02d.%02d %02d:%02d:%02d",
                                 1900+bt.tm_year, 1+bt.tm_mon, bt.tm_mday,
                                 bt.tm_hour,      bt.tm_min,   bt.tm_sec) != sizeof(tmp)-1 )
    throw ExceptionConversionError(SYSTEM_SAVE_LOCATION, "snprintf() wrote invalid number of characters");

  assert( tmp[sizeof(tmp)-1]==0 );
  return tmp;
}

std::ostream &operator<<(std::ostream &os, const Timestamp ts)
{
  os<<ts.get();
  return os;
}

} // namespace Persistency
