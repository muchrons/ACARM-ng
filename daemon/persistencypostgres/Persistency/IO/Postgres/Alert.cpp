/*
 * Alert.hpp
 *
 */
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

using namespace Persistency::IO::Postgres::detail;

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
  const Persistency::HostPtr host=anlz.getHost();
  EntrySaver es(t,*dbHandler_);

  Persistency::Alert::ReportedHosts SourceHosts(a.getReportedSourceHosts() );
  Persistency::Alert::ReportedHosts TargetHosts(a.getReportedTargetHosts() );
  //save Host
  DataBaseID hostID = es.saveHostData(*host.get());
  //save Analyzer
  DataBaseID anlzID = es.saveAnalyzer(&hostID, anlz);
  //save Alert
  DataBaseID alertID = es.saveAlert(anlzID, a);
  //TODO reorganize code
  Persistency::Alert::ReportedHosts::iterator it;
  for(it = SourceHosts.begin(); it!=SourceHosts.end() ; it++)
  {
    DataBaseID hostID = es.saveHostData(*it->get() );
    DataBaseID reportedHostID = es.saveSourceHost(hostID, alertID, *it->get() );
    //get reported services from source host
    Persistency::Host::ReportedServices Services((*it->get()).getReportedServices() );
    //get reported processes from source host
    Persistency::Host::ReportedProcesses Processes((*it->get()).getReportedProcesses() );

    //save reported services for source host
    Persistency::Host::ReportedServices::iterator it_s;
    for(it_s = Services.begin(); it_s!=Services.end(); it_s++)
    {
      es.saveService(reportedHostID, *it_s->get() );
      //TODO
    }
    //save reported processes for source host
    Persistency::Host::ReportedProcesses::iterator it_p;
    for(it_p = Processes.begin(); it_p!=Processes.end(); it_p++)
    {
      //TODO
    }
  }

  //save target hosts
  for(it = TargetHosts.begin(); it!=TargetHosts.end() ; it++)
  {
    DataBaseID hostID = es.saveHostData(*it->get() );
    DataBaseID reportedHostID = es.saveTargetHost(hostID, alertID, *it->get() );
    //get reported processes from target host
    Persistency::Host::ReportedProcesses Processes((*it->get()).getReportedProcesses() );
    //get reported services from target host
    Persistency::Host::ReportedServices Services((*it->get()).getReportedServices() );
    Persistency::Host::ReportedServices::iterator it_s;
    for(it_s = Services.begin(); it_s!=Services.end(); it_s++)
    {
     //TODO
    }
    //save reported processes
    Persistency::Host::ReportedProcesses::iterator it_p;
    for(it_p = Processes.begin(); it_p!=Processes.end(); it_p++)
    {
      //TODO
    }
  }

  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
