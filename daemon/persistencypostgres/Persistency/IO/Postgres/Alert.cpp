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
  log_("persistency.io.postgres.alert"),
  dbHandler_(dbHandler)
{
}

void Alert::saveImpl(Transaction &t)
{
  EntrySaver                 es(t,*dbHandler_);
  //get Alert
  const Persistency::Alert  &a=*get();
  //save Alert
  DataBaseID                 alertID=es.saveAlert(a);
  //add Alert to cache
  dbHandler_->getIDCache()->add(get() , alertID);
  //save source hosts
  Persistency::Alert::ReportedHosts SourceHosts( a.getReportedSourceHosts() );
  saveHosts(es, alertID, HostType::SRC, SourceHosts);
  //save target hosts
  Persistency::Alert::ReportedHosts TargetHosts( a.getReportedTargetHosts() );
  saveHosts(es, alertID, HostType::DST, TargetHosts);
  //get Analyzers from Alert
  Persistency::Alert::SourceAnalyzers analyzers( a.getSourceAnalyzers() );
  //save Analyzers
  for(Persistency::Alert::SourceAnalyzers::iterator it = analyzers.begin(); it != analyzers.end(); ++it)
  {
    //save Analyzer
    const Persistency::Analyzer &analyzer = *it->get();
    const DataBaseID             anlzID = es.saveAnalyzer(analyzer);
    es.saveAlertToAnalyzers(alertID, anlzID);
  }

}

void Alert::saveHosts(EntrySaver                        &es,
                      DataBaseID                         alertID,
                      HostType                           type,
                      Persistency::Alert::ReportedHosts &hosts)
{
  for(Persistency::Alert::ReportedHosts::iterator it = hosts.begin(); it!=hosts.end() ; ++it)
  {
    const DataBaseID hostID = es.saveHostData(*it->get() );
    dbHandler_->getIDCache()->add(*it , hostID);
    DataBaseID       reportedHostID;
    // save reported host
    switch( type.toInt() )
    {
      case HostType::SRC:
        LOGMSG_DEBUG_S(log_)<<"save source host with ID: "<<hostID;
        reportedHostID = es.saveSourceHost(hostID, alertID, *it->get() );
      break;

      case HostType::DST:
        LOGMSG_DEBUG_S(log_)<<"save target host with ID: "<<hostID;
        reportedHostID = es.saveTargetHost(hostID, alertID, *it->get() );
      break;
      default:
        reportedHostID = 0;
        assert(!"never reach here");
      break;
    }

    // get reported services from host
    const Persistency::Host::ReportedServices  &services( (*it->get()).getReportedServices() );
    LOGMSG_DEBUG_S(log_)<<"save reported services for alert with ID: "<<alertID;
    for(Persistency::Host::ReportedServices::const_iterator it_s = services.begin(); it_s!=services.end(); ++it_s)
      es.saveService(reportedHostID, *it_s->get() );

    // get reported processes from host
    const Persistency::Host::ReportedProcesses &processes((*it->get()).getReportedProcesses() );
    LOGMSG_DEBUG_S(log_)<<"save reported processes for alert with ID: "<<alertID;
    for(Persistency::Host::ReportedProcesses::const_iterator it_p = processes.begin(); it_p!=processes.end(); ++it_p)
      es.saveProcess(reportedHostID, *it_p->get());
  }
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
