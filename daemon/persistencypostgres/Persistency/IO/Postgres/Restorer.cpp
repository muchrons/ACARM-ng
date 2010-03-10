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
  //IO::ConnectionPtrNN connectionStubIO( createStubIO() );
  //IO::Transaction tStubIO( connectionStubIO->createNewTransaction("stub transaction") );
  // TODO
  // read meta alerts ids from table meta_alerts_in_use
  // for(meta alerts in use)
  // {
  //   - create GraphNode
  //     - read Meta Alert
  //     - read Meta Alert Children
  //     - leafs should be read first
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

BackendFactory::FactoryPtr Restorer::createStubIO(void)
{
  const BackendFactory::FactoryTypeName name("stubx");
  const BackendFactory::Options         options;
  return BackendFactory::create(name, options);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
