/*
 * Alert.hpp
 *
 */
#include "Persistency/IO/Postgres/Alert.hpp"

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

  DataBaseID hostID;
  DataBaseID anlzID;
  //save Host
  if(host!=NULL)
  {
    hostID = es.saveHostData(*host.get());
    anlzID = es.saveAnalyzer(&hostID, anlz);
  }
  else
    anlzID = es.saveAnalyzer(NULL, anlz);
  //save Alert
  DataBaseID alertID = es.saveAlert(anlzID, a);
  //save source hosts
  Persistency::Alert::ReportedHosts SourceHosts(a.getReportedSourceHosts() );
  PtrEntrySaver saveSourceHostPtr = &EntrySaver::saveSourceHost;
  saveHosts(es, alertID, saveSourceHostPtr, SourceHosts);
  //save target hosts
  Persistency::Alert::ReportedHosts TargetHosts(a.getReportedTargetHosts() );
  PtrEntrySaver saveTargetHostPtr = &EntrySaver::saveTargetHost;
  saveHosts(es, alertID, saveTargetHostPtr, TargetHosts);

}

void Alert::saveHosts(EntrySaver                 &es,
               DataBaseID                        alertID,
               PtrEntrySaver                     Ptr,
               Persistency::Alert::ReportedHosts &Hosts)
{
  Persistency::Alert::ReportedHosts::iterator it;
  for(it = Hosts.begin(); it!=Hosts.end() ; it++)
  {
    DataBaseID hostID = es.saveHostData(*it->get() );
    // save reported host
    DataBaseID reportedHostID = (es.*Ptr)(hostID, alertID, *it->get() );
    //get reported services from host
    Persistency::Host::ReportedServices Services((*it->get()).getReportedServices() );
    //get reported processes from host
    Persistency::Host::ReportedProcesses Processes((*it->get()).getReportedProcesses() );
    //save reported services for host
    Persistency::Host::ReportedServices::iterator it_s;
    for(it_s = Services.begin(); it_s!=Services.end(); it_s++)
    {
      es.saveService(reportedHostID, *it_s->get() );
    }
    //save reported processes for host
    Persistency::Host::ReportedProcesses::iterator it_p;
    for(it_p = Processes.begin(); it_p!=Processes.end(); it_p++)
    {
      es.saveProcess(reportedHostID, *it_p->get());
    }
  }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
