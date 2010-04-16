/*
 * Timestamp.cpp
 *
 */
#include <ctime>
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

std::ostream &operator<<(std::ostream &os, const Timestamp ts)
{
  os<<ts.get();
  return os;
}

} // namespace Persistency
