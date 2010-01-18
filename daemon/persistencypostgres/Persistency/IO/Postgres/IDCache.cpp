/*
 * IDCache.cpp
 *
 */

#include "Persistency/IO/Postgres/IDCache.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

void IDCache::prune(void)
{
  hostsIDs_.prune();
  alertsIDs_.prune();
  metaAlertsIDs_.prune();
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
