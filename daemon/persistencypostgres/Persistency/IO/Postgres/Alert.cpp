/*
 * Alert.hpp
 *
 */
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

using namespace Persistency::IO::Postgres::detail;
using namespace std;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Alert::Alert(Persistency::AlertPtrNN  alert,
             Transaction             &t,
             DBHandlerPtrNN           dbHandler):
  IO::Alert(alert, t),
  dbHandler_(dbHandler)
{
}

void Alert::saveImpl(Transaction &t)
{
  const Persistency::Alert &a=get();
  const Persistency::Analyzer &anlz=a.getAnalyzer();
  EntrySaver es(t,*dbHandler_);

  vector<HostPtrNN> SourceHosts(a.getReportedSourceHosts() );
  vector<HostPtrNN> TargetHosts(a.getReportedTargetHosts() );


  for(vector<HostPtrNN>::iterator it = SourceHosts.begin(); it!=SourceHosts.end() ; it++)
  {
    DataBaseID hostID = es.saveHostData(*it->get() );
    DataBaseID anlzID = es.saveAnalyzer(&hostID, anlz);
    DataBaseID alertID = es.saveAlert(anlzID, a);
    es.saveSourceHost(hostID, alertID, *it->get() );
  }

  for(vector<HostPtrNN>::iterator it = TargetHosts.begin(); it!=TargetHosts.end() ; it++)
  {
    DataBaseID hostID = es.saveHostData(*it->get() );
    DataBaseID anlzID = es.saveAnalyzer(&hostID, anlz);
    DataBaseID alertID = es.saveAlert(anlzID, a);
    es.saveTargetHost(hostID, alertID, *it->get() );
  }

  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
