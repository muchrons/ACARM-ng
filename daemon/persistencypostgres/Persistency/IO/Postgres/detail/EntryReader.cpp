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
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using namespace std;
using namespace pqxx;

using boost::algorithm::trim;
using Persistency::IO::Transaction;

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

SeverityLevel severityFromInt(int level)
{
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

template <typename T>
inline pqxx::result execSQL(Transaction &t, const T &sql)
{
  return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()

} // unnamed namespace


EntryReader::EntryReader(Transaction &t, DBHandler &dbh):
  dbh_(dbh),
  t_(t)
{
}

//TODO: work in progress
// write tests !!
Persistency::AlertPtrNN EntryReader::readAlert(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alerts WHERE id = " << alertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  AlertPtrNN alert( new Alert(ReaderHelper<string>::fromSQLResult(r[0]["name"]),
                              getAnalyzers( alertID ),
                              ReaderHelper<Timestamp>::readAs(r[0]["detect_time"]).get(),
                              timestampFromString( ReaderHelper<string>::fromSQLResult(r[0]["create_time"]) ),
                              Severity( severityFromInt( ReaderHelper<int>::fromSQLResult(r[0]["id_severity"]) ) ),
                              Certainty( ReaderHelper<double>::fromSQLResult(r[0]["certanity"]) ),
                              ReaderHelper<string>::fromSQLResult(r[0]["description"]),
                              getSourceHosts( alertID ),
                              getTargetHosts( alertID ) ) );
  return alert;
}

Persistency::MetaAlertPtrNN EntryReader::readMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  MetaAlertPtrNN malert( new Persistency::MetaAlert( ReaderHelper<string>::fromSQLResult(r[0]["name"]),
                                          ReaderHelper<double>::fromSQLResult(r[0]["severity_delta"]),
                                          ReaderHelper<double>::fromSQLResult(r[0]["certanity_delta"]),
                                          getReferenceURL( ReaderHelper<DataBaseID,Base::NullValue<DataBaseID> ,DataBaseID>::readAs(r[0]["id_ref"]).get() ),
                                          timestampFromString( ReaderHelper<string>::fromSQLResult(r[0]["create_time"]) )) );
  return malert;
}

// TODO: tests
AnalyzerPtrNN EntryReader::getAnalyzer(DataBaseID anlzID)
{
  stringstream sa;
  sa << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  const result ra = execSQL(t_, sa);
  if(ra.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, sa.str());
  AnalyzerPtrNN anlz(new Analyzer( Analyzer::Name( ReaderHelper<string>::fromSQLResult(ra[0]["name"]) ),
                                   ReaderHelper<Analyzer::Version>::readAs(ra[0]["version"] ).get(),
                                   ReaderHelper<Analyzer::OS>::readAs(ra[0]["os"] ).get(),
                                   ReaderHelper<Analyzer::IP>::readAs(ra[0]["ip"] ).get() ));
  return anlz;
}

Alert::SourceAnalyzers EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  const result r = execSQL(t_, ss);

  Alert::SourceAnalyzers analyzers( getAnalyzer( ReaderHelper<DataBaseID>::fromSQLResult(r[0]["id_analyzer"]) ) );

  for(size_t i=0; i<r.size(); ++i)
    analyzers.push_back( getAnalyzer( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id_analyzer"]) ) );
  return analyzers;
}

Alert::ReportedHosts EntryReader::getReporteHosts(DataBaseID alertID, std::string hostType)
{
  stringstream ss;
  ss << "SELECT * FROM reported_hosts WHERE id_alert = "<< alertID <<" AND role = ";
  Appender::append(ss, hostType);
  ss << ";";
  const result r = execSQL(t_, ss);
  Alert::ReportedHosts hosts;
  for(size_t i=0; i<r.size(); ++i)
  {
    DataBaseID idRefURL;
    r[i]["id_ref"].to(idRefURL);
    hosts.push_back(getHost( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id_host"]),
                             &idRefURL /*fromSQLResult<DataBaseID>(r[i]["id_ref"])*/ ));
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
  const result r = execSQL(t_, ss);

  HostPtr host(new Persistency::Host(Persistency::Host::IP::from_string(ReaderHelper<string>::fromSQLResult(r[0]["ip"])),
                                     ReaderHelper<Persistency::Host::Netmask>::readAs(r[0]["mask"]).get(),
                                     ReaderHelper<string, Persistency::Host::OperatingSystem>::readAs(r[0]["os"]),
                                     getReferenceURL(refID),
                                     getReportedServices( hostID ),
                                     getReportedProcesses( hostID ),
                                     ReaderHelper<string, Persistency::Host::Name>::readAs(r[0]["name"]) ) );
  // add host to cache
  dbh_.getIDCache()->add(host , hostID);
  return host;
}

Persistency::Host::ReportedServices EntryReader::getReportedServices(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM reported_services WHERE id_reported_host = " << hostID << ";";
  const result r = execSQL(t_, ss);

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
  const result r = execSQL(t_, ss);

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
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  Persistency::ServicePtrNN service(new Persistency::Service(ReaderHelper<string>::fromSQLResult(r[0]["name"]),
                                                             ReaderHelper<DataBaseID>::fromSQLResult(r[0]["port"]),
                                                             ReaderHelper<string, Persistency::Service::Protocol>::readAs(r[0]["protocol"]),
                                                           getReferenceURL( refID )));
  return service;
}

ProcessPtr EntryReader::getProcess(DataBaseID procID, DataBaseID *refID)
{
  stringstream ss;
  ss << "SELECT * FROM processes WHERE id = " << procID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  string md5;
  // TODO: consider using NullValue class for this.
  MD5Sum *procMD5 = NULL;
  if( !r[0]["md5"].is_null() )
  {
    r[0]["md5"].to(md5);
    // TODO: SEGV here - procMD5 is NULL in this context
    // smart pointer
    // TODO: copy c-tor not needed here
    *procMD5 = MD5Sum( MD5Sum::createFromString(md5.c_str()) );
  }

  stringstream sr;
  sr << "SELECT * FROM reported_procs WHERE id_proc = " << procID << ";";
  const result rr = execSQL(t_, sr);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  string arguments;
  rr[0]["arguments"].to(arguments);

  Persistency::ProcessPtr process( new Process( ReaderHelper<string,Process::Path>::readAs(r[0]["path"]),
                                                ReaderHelper<string>::fromSQLResult(r[0]["name"]),
                                                procMD5,
                                                ReaderHelper<pid_t, Base::NullValue<pid_t>, int>::readAs(rr[0]["pid"]).get(),
                                                ReaderHelper<int, Base::NullValue<int>, int>::readAs(rr[0]["uid"]).get(),
                                                ReaderHelper<string>::fromSQLResult(rr[0]["username"]),
                                                arguments.c_str(),
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
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());

  refURLPtr = ReferenceURLPtr( new ReferenceURL(ReaderHelper<string>::fromSQLResult(r[0]["name"]),
                                                ReaderHelper<string>::fromSQLResult(r[0]["url"])) );
  return refURLPtr;
}

double EntryReader::getSeverityDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT severity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::fromSQLResult(r[0]["severity_delta"]);
}
double EntryReader::getCertaintyDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT caertanity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<double>::fromSQLResult(r[0]["certanity_delta"]);
}

Persistency::AlertPtrNN EntryReader::getLeaf(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return Persistency::AlertPtrNN( readAlert( ReaderHelper<DataBaseID>::fromSQLResult(r[0]["id_alert"]) ) );
}

vector<DataBaseID> EntryReader::readMetaAlertChildren(DataBaseID malertID)
{
  vector<DataBaseID> childrenIDs;
  stringstream ss;
  ss << "SELECT id_child FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  const result r = execSQL(t_, ss);

  for(size_t i=0; i<r.size(); ++i)
    childrenIDs.push_back( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id_child"]) );
  return childrenIDs;
}

vector<DataBaseID> EntryReader::readIDsMalertsInUse()
{
  vector<DataBaseID> malertsInUse;
  stringstream ss;
  ss << "SELECT id_meta_alert FROM meta_alerts_in_use;";
  const result r = execSQL(t_, ss);
  malertsInUse.reserve( r.size() );
  for(size_t i=0; i<r.size(); ++i)
  {
    //assert that id_meta_alert is not NULL
    assert(!r[i]["id_meta_alert"].is_null());
    malertsInUse.push_back( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id_meta_alert"]) );
  }
  return malertsInUse;
}

vector<DataBaseID> EntryReader::readIDsMalertsBetween(const Timestamp &from, const Timestamp &to)
{
  vector<DataBaseID> malertsBetween;
  stringstream       ss;
  //TODO: test this query
  ss << "SELECT id FROM meta_alerts WHERE ";
  Appender::append(ss, from);
  ss << " <= create_time AND create_time <=";
  Appender::append(ss, to);
  const result r = execSQL(t_, ss);

  for(size_t i=0; i<r.size(); ++i)
    malertsBetween.push_back( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id"]) );
  return malertsBetween;
}

std::vector<DataBaseID> EntryReader::readRoots()
{
  vector<DataBaseID> roots;
  execSQL(t_,"SELECT id_node, id_child INTO TEMP TABLE tmp FROM meta_alerts_tree"
             " INNER JOIN meta_alerts_in_use ON(meta_alerts_tree.id_node=meta_alerts_in_use.id_meta_alert);");

  const result r = execSQL(t_, "SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( "
                         "SELECT 1 FROM tmp S WHERE T.id_node=S.id_child );");

  // TODO: this should be ensured on transaction level (use 'ON COMMIT DROP' feature).
  execSQL(t_, "DROP TABLE tmp;");
  for(size_t i=0; i<r.size(); ++i)
  {
    // assert to ensure value is not null
    assert(!r[i]["id_node"].is_null());
    roots.push_back( ReaderHelper<DataBaseID>::fromSQLResult(r[i]["id_node"]) );
  }
  return roots;
}

DataBaseID EntryReader::getAlertIDAssociatedWithMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = execSQL(t_, ss);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return ReaderHelper<DataBaseID>::fromSQLResult(r[0]["id_alert"]);
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
