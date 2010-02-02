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
