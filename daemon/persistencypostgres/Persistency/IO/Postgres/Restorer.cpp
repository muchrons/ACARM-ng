/*
 * Restorer.cpp
 *
 */
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include <map>
#include <vector>
using namespace Persistency::IO::Postgres::detail;
using namespace std;
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

void Restorer::restoreAllInUseImpl(Transaction &t, NodesVector &/*out*/)
{
  EntryReader er(t, *dbHandler_);
  vector<DataBaseID> maInUse( er.readIDsMalertsInUse() );

  for(vector<DataBaseID>::iterator it = maInUse.begin(); it != maInUse.end(); ++it)
  {
    vector<DataBaseID> malertChildren( er.readMetaAlertChildren( (*it) ) );
    // put this data to the tree which represents meta alerts tree structure
    //
  }

  // an old solutions, ideas
  /*
  std::map<DataBaseID, Persistency::AlertPtrNN>           leafsAlertMap;
  er.getLeafs(leafsAlertMap);

  IO::ConnectionPtrNN connectionStubIO( createStubIO() );
  IO::Transaction tStubIO( connectionStubIO->createNewTransaction("stub transaction") );

  leafsMap leafs;
  for(std::map<DataBaseID, Persistency::AlertPtrNN>::iterator it = leafsAlertMap.begin();
      it != leafsAlertMap.end(); ++it)
  {
    leafs.insert( std::pair<DataBaseID, Persistency::GraphNodePtrNN>( ((*it).first()),
                  Persistency::GraphNodePtrNN( new Persistency::GraphNode( ((*it).second()),
                                                              connectionStubIO,
                                                              tStubIO) ) ) );
  }
  */
  // nodesMultimap nodes;
  // multimap<DataBaseID, pair<DataBaseID, GraphNode> > nodesMultimap;
  // TODO
  // read meta alerts ids from table meta_alerts_in_use
  // for(meta alerts in use)
  // {
  //   - create GraphNode
  //     - read Meta Alert
  //     - read Meta Alert Children
  //     - leafs should be read first
  //     - guery to select leafs ids
  //     SELECT id_alert FROM alert_to_meta_alert_map
  //     INNER JOIN meta_alerts_in_use ON
  //     (alert_to_meta_alert_map.id_meta_alert = meta_alerts_in_use.id_meta_alert);
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
