/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Postgres/Restorer.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Restorer::Restorer(Transaction    &t,
                   DBHandlerPtrNN  dbHandler):
  IO::Restorer(t),
  dbHandler_(dbHandler)
{
}

void Restorer::restoreAllInUseImpl(Transaction &/*t*/, NodesVector &/*out*/)
{
  // TODO
  // read meta alerts ids from table meta_alerts_in_use
  // for(meta alerts in use)
  // {
  //   - create GraphNode
  //     - read Meta Alert
  //     - read Meta Alert Children:w
  //
  //   - add GraphNode to NodesVector
  // }
}

void Restorer::restoreBetweenImpl(Transaction     &/*t*/,
                                  NodesVector     &/*out*/,
                                  const Timestamp &/*from*/,
                                  const Timestamp &/*to*/)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
