/*
 * EntryReader.cpp
 *
 */
#include <sstream>
#include <ctime>
#include <cassert>
#include <boost/algorithm/string.hpp>

#include "Base/NullValue.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/SQLHelper.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using namespace std;
using namespace pqxx;

using boost::algorithm::trim;
using namespace Persistency;
using namespace Base;
using Persistency::IO::Transaction;

// this is helper macro for calling f-cjtion that saves line number and calls given sql statement (with log)
#define SQL(sql, log) SQLHelper(__FILE__, __LINE__, (sql), (log))

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

namespace
{

SeverityLevel severityFromInt(Transaction &t, const DataBaseID id)
{
  // prepare SQL query
  stringstream ss;
  ss<<"SELECT level FROM severities WHERE id="<<id;
  // execute it
  const result r=t.getAPI<Postgres::TransactionAPI>().exec(ss);
  if( r.size()!=1 )
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str() );
  // return read value as a number
  const int level=ReaderHelper<int>::readAsNotNull(r[0]["level"]);

  // translate to enum
  switch(level)
  {
    case 0: return SeverityLevel::DEBUG;
    case 1: return SeverityLevel::INFO;
    case 2: return SeverityLevel::NOTICE;
    case 3: return SeverityLevel::WARNING;
    case 4: return SeverityLevel::PROBLEM;
    case 5: return SeverityLevel::ERROR;
    case 6: return SeverityLevel::CRITICAL;
  }
  // when we reach here, there is wrong severity level in data base
  assert(!"invalid severity level");
  return SeverityLevel::CRITICAL;
}

} // unnamed namespace


EntryReader::EntryReader(Transaction &t, DBHandler &dbh):
  log_("persistency.io.postgres.detail.entryreader"),
  dbh_(dbh),
  t_(t)
{
}

Persistency::AlertPtrNN EntryReader::readAlert(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alerts WHERE id = " << alertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  AlertPtrNN alert( new Alert(ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                              getAnalyzers( alertID ),
                              ReaderHelper< Base::NullValue<Timestamp> >::readAs(r[0]["detect_time"]).get(),
                              timestampFromString( ReaderHelper<string>::readAsNotNull(r[0]["create_time"]) ),
                              Severity( severityFromInt(t_, ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_severity"]) ) ),
                              Certainty( ReaderHelper<double>::readAsNotNull(r[0]["certanity"]) ),
                              ReaderHelper<string>::readAsNotNull(r[0]["description"]),
                              getSourceHosts( alertID ),
                              getTargetHosts( alertID ) ) );
  return alert;
}

Persistency::MetaAlertPtrNN EntryReader::readMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  const string CreateTime( ReaderHelper<string>::readAsNotNull(r[0]["create_time"]) );
  MetaAlertPtrNN malert( new Persistency::MetaAlert( ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                          ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]),
                                          ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]),
                                          getReferenceURL( ReaderHelper< NullValue<DataBaseID> >::readAs(r[0]["id_ref"]).get() ),
                                          timestampFromString( CreateTime ) ) );
  return malert;
}

AnalyzerPtrNN EntryReader::getAnalyzer(DataBaseID anlzID)
{
  stringstream sa;
  sa << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  const result ra = SQL( sa.str(), log_ ).exec(t_);
  if(ra.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, sa.str());
  AnalyzerPtrNN anlz(new Analyzer( Analyzer::Name( ReaderHelper<string>::readAsNotNull(ra[0]["name"]) ),
                                   ReaderHelper<Analyzer::Version>::readAs(ra[0]["version"]),
                                   ReaderHelper<Analyzer::OperatingSystem>::readAs(ra[0]["os"]),
                                   ReaderHelper< Base::NullValue<Analyzer::IP> >::readAs(ra[0]["ip"]).get() ));
  return anlz;
}

Alert::SourceAnalyzers EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  // add first element to the Analyzers
  Alert::SourceAnalyzers analyzers( getAnalyzer( ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_analyzer"]) ) );
  // enumerating starts from '1' because first element was added to the Analyzers above
  for(size_t i=1; i<r.size(); ++i)
    analyzers.push_back( getAnalyzer( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_analyzer"]) ) );
  return analyzers;
}

Alert::ReportedHosts EntryReader::getReporteHosts(DataBaseID alertID, std::string hostType)
{
  stringstream ss;
  ss << "SELECT * FROM reported_hosts WHERE id_alert = "<< alertID <<" AND role = ";
  Appender::append(ss, hostType);
  const result r = SQL( ss.str(), log_ ).exec(t_);
  Alert::ReportedHosts hosts;
  for(size_t i=0; i<r.size(); ++i)
  {
    DataBaseID idRefURL = ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_ref"]);
    hosts.push_back(getHost( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_host"]),
                             &idRefURL));
  }
  return hosts;
}

Alert::ReportedHosts EntryReader::getSourceHosts(DataBaseID alertID)
{
  return getReporteHosts(alertID, "src");
}

Alert::ReportedHosts EntryReader::getTargetHosts(DataBaseID alertID)
{
  return getReporteHosts(alertID, "dst");
}

HostPtr EntryReader::getHost(DataBaseID hostID, DataBaseID *refID)
{
  stringstream ss;
  ss << "SELECT * FROM hosts WHERE id = "<< hostID <<";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  HostPtr host(new Persistency::Host(ReaderHelper<Persistency::Host::IP>::readAsNotNull(r[0]["ip"]),
                                     ReaderHelper< Base::NullValue<Persistency::Host::Netmask> >::readAs(r[0]["mask"]).get(),
                                     ReaderHelper<Persistency::Host::OperatingSystem>::readAs(r[0]["os"]),
                                     getReferenceURL(refID),
                                     getReportedServices( hostID ),
                                     getReportedProcesses( hostID ),
                                     ReaderHelper<Persistency::Host::Name>::readAs(r[0]["name"]) ) );
  // add host to cache
  addIfNew(host, hostID);
  return host;
}

Persistency::Host::ReportedServices EntryReader::getReportedServices(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM reported_services WHERE id_reported_host = " << hostID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  Persistency::Host::ReportedServices services;
  for(size_t i = 0;i < r.size(); ++i)
  {
    DataBaseID idService, idRef;
    r[i]["id_service"].to(idService);
    r[i]["id_ref"].to(idRef);
    services.push_back( getService(idService, &idRef) );
  }
  return services;
}

Persistency::Host::ReportedProcesses EntryReader::getReportedProcesses(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM reported_procs WHERE id_reported_host = " << hostID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);

  Persistency::Host::ReportedProcesses processes;
  DataBaseID idProcess, idRef;
  for(size_t i = 0; i<r.size(); ++i)
  {
    r[i]["id_proc"].to(idProcess);
    r[i]["id_ref"].to(idRef);
    processes.push_back( getProcess(idProcess, &idRef) );
  }
  return processes;
}

Persistency::ServicePtrNN EntryReader::getService(DataBaseID servID, DataBaseID *refID)
{
  stringstream ss;
  ss << "SELECT * FROM services WHERE id = " << servID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  ServicePtrNN service(new Service(ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                   ReaderHelper<DataBaseID>::readAsNotNull(r[0]["port"]),
                                   ReaderHelper<Service::Protocol>::readAs(r[0]["protocol"]),
                                   getReferenceURL( refID )));
  return service;
}

ProcessPtr EntryReader::getProcess(DataBaseID procID, DataBaseID *refID)
{
  stringstream ss;
  ss << "SELECT * FROM processes WHERE id = " << procID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());

  stringstream sr;
  sr << "SELECT * FROM reported_procs WHERE id_proc = " << procID << ";";
  const result rr = SQL( sr.str(), log_ ).exec(t_);
  if(rr.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  const NullValue<string> params=ReaderHelper< NullValue<string> >::readAs(rr[0]["arguments"]);
  Persistency::ProcessPtr process( new Process( ReaderHelper<Process::Path>::readAs(r[0]["path"]),
                                                ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                                ReaderHelper< std::auto_ptr<MD5Sum> >::readAs(r[0]["md5"]).get(),
                                                ReaderHelper< NullValue<pid_t> >::readAs(rr[0]["pid"]).get(),
                                                ReaderHelper< NullValue<int> >::readAs(rr[0]["uid"]).get(),
                                                ReaderHelper<string>::readAsNotNull(rr[0]["username"]),
                                                (params.get()==NULL)?NULL:params.get()->c_str(),
                                                getReferenceURL( refID )) );
  return process;
}

ReferenceURLPtr EntryReader::getReferenceURL(const DataBaseID *refID)
{
  ReferenceURLPtr refURLPtr;
  if(refID == NULL)
    return refURLPtr;
  stringstream ss;
  ss << "SELECT * FROM reference_urls WHERE id = " << *refID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());

  refURLPtr = ReferenceURLPtr( new ReferenceURL(ReaderHelper<string>::readAsNotNull(r[0]["name"]),
                                                ReaderHelper<string>::readAsNotNull(r[0]["url"])) );
  return refURLPtr;
}

double EntryReader::getSeverityDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT severity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::readAsNotNull(r[0]["severity_delta"]);
}
double EntryReader::getCertaintyDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT certainty_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::readAsNotNull(r[0]["certainty_delta"]);
}

Persistency::AlertPtrNN EntryReader::getLeaf(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return Persistency::AlertPtrNN( readAlert( ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_alert"]) ) );
}

vector<DataBaseID> EntryReader::readMetaAlertChildren(DataBaseID malertID)
{
  vector<DataBaseID> childrenIDs;
  stringstream ss;
  ss << "SELECT id_child FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  childrenIDs.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
    childrenIDs.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_child"]) );
  return childrenIDs;
}

vector<DataBaseID> EntryReader::readIDsMalertsInUse()
{
  vector<DataBaseID> malertsInUse;
  stringstream ss;
  ss << "SELECT id_meta_alert FROM meta_alerts_in_use;";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  malertsInUse.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
  {
    //assert that id_meta_alert is not NULL
    assert(!r[i]["id_meta_alert"].is_null());
    malertsInUse.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_meta_alert"]) );
  }
  return malertsInUse;
}

vector<DataBaseID> EntryReader::readIDsMalertsBetween(const Timestamp &from, const Timestamp &to)
{
  vector<DataBaseID> malertsBetween;
  stringstream       ss;
  ss << "SELECT id FROM meta_alerts WHERE ";
  Appender::append(ss, from);
  ss << " <= create_time AND create_time <=";
  Appender::append(ss, to);
  ss << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  malertsBetween.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
    malertsBetween.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id"]) );
  return malertsBetween;
}

std::vector<DataBaseID> EntryReader::readRoots()
{
  SQL("CREATE TEMP TABLE tmp ON COMMIT DROP AS SELECT id_node, id_child FROM meta_alerts_tree"
      " INNER JOIN meta_alerts_in_use ON(meta_alerts_tree.id_node=meta_alerts_in_use.id_meta_alert);", log_).exec(t_);

  const result r = SQL("SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( "
                       "SELECT id_node FROM tmp S WHERE T.id_node=S.id_child );", log_).exec(t_);

  return getRoots(r);
}

std::vector<DataBaseID> EntryReader::readRoots(const Timestamp &from, const Timestamp &to)
{
  stringstream ss;
  ss << "CREATE TEMP TABLE tmp ON COMMIT DROP AS SELECT id_node, id_child FROM meta_alerts_tree"
        " INNER JOIN meta_alerts ON(meta_alerts_tree.id_node=meta_alerts.id) WHERE ";
  Appender::append(ss, from);
  ss << " <= create_time AND create_time <=";
  Appender::append(ss, to);
  ss << ";";
  SQL( ss.str(), log_ ).exec(t_);

  const result r = SQL("SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( "
                       "SELECT id_node FROM tmp S WHERE T.id_node=S.id_child );", log_).exec(t_);

  return getRoots(r);
}

DataBaseID EntryReader::getAlertIDAssociatedWithMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id_alert"]);
}

template<typename T>
void EntryReader::addIfNew(T e, DataBaseID id)
{
  if(!dbh_.getIDCache()->has(e))
    dbh_.getIDCache()->add(e, id);
  assert(id == dbh_.getIDCache()->get(e));
}

std::vector<DataBaseID> EntryReader::getRoots(const pqxx::result &r)
{
  vector<DataBaseID> roots;
  roots.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
  {
    // assert to ensure value is not null
    assert(!r[i]["id_node"].is_null());
    roots.push_back( ReaderHelper<DataBaseID>::readAsNotNull(r[i]["id_node"]) );
  }
  return roots;
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
