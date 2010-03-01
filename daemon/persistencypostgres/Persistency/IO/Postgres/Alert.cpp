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
  // TODO: THIS IS COMMENTED OUT SINCE IMPLEMENTATION OF ANALYZER CHANGED - UPDATE THIS CODE ASAP
  EntrySaver                             es(t,*dbHandler_);
  //get Alert
  const Persistency::Alert               &a=get();
  //save Alert
  const DataBaseID                       alertID=es.saveAlert(a);
  //save source hosts
  Persistency::Alert::ReportedHosts      SourceHosts(a.getReportedSourceHosts() );
  saveHosts(es, alertID, SRC, SourceHosts);
  //save target hosts
  Persistency::Alert::ReportedHosts      TargetHosts(a.getReportedTargetHosts() );
  saveHosts(es, alertID, DST, TargetHosts);
  //get Analyzers from Alert
  Persistency::Alert::SourceAnalyzers    analyzers(a.getSourceAnalyzers() );
  //save Analyzers
  for(Persistency::Alert::SourceAnalyzers::iterator it = analyzers.begin(); it != analyzers.end(); ++it)
  {
    const Persistency::Analyzer &analyzer = *it->get();
    //save Analyzer
    const DataBaseID            anlzID = es.saveAnalyzer(analyzer );
    es.saveAlertToAnalyzers(alertID, anlzID);
  }

}

void Alert::saveHosts(EntrySaver                        &es,
                      DataBaseID                         alertID,
                      host_type                          type,
                      Persistency::Alert::ReportedHosts &hosts)
{
  for(Persistency::Alert::ReportedHosts::iterator it = hosts.begin(); it!=hosts.end() ; ++it)
  {
    const DataBaseID hostID = es.saveHostData(*it->get() );
    DataBaseID       reportedHostID;
    // save reported host
    switch(type)
    {
      case SRC:
        reportedHostID = es.saveSourceHost(hostID, alertID, *it->get() );
      break;
      case DST:
        reportedHostID = es.saveTargetHost(hostID, alertID, *it->get() );
      break;
      // TODO: add default case with proper assertion
    }

    // get reported services from host
    // TODO: this variable should be const reference
    // TODO: keep variable names lowercase
    Persistency::Host::ReportedServices Services((*it->get()).getReportedServices() );
    for(Persistency::Host::ReportedServices::iterator it_s = Services.begin(); it_s!=Services.end(); ++it_s)
      es.saveService(reportedHostID, *it_s->get() );

    // get reported processes from host
    // TODO: this variable should be const reference
    // TODO: keep variable names lowercase
    Persistency::Host::ReportedProcesses Processes((*it->get()).getReportedProcesses() );
    for(Persistency::Host::ReportedProcesses::iterator it_p = Processes.begin(); it_p!=Processes.end(); ++it_p)
      es.saveProcess(reportedHostID, *it_p->get());
  }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
