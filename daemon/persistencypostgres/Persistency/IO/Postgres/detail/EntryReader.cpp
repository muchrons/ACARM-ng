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
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"
#include "Persistency/IO/Postgres/detail/fromSQLResult.hpp"

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

// TODO: set() is not accurate gere - get() or read() (readAs()?) would be better here
template<typename T, typename S>
Base::NullValue<T> set(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<T> ret;
    return ret;
  }
  else
  {
    S s;
    r.to(s);
    T data(s);
    // TODO: memory leak here. should be ret(&s).
    Base::NullValue<T> ret( data );
    return ret;
  }
}

// TODO: make this partial specialization of set() method.
// NOTE: partial specialization is a bit tricky - you may want to make class with
//       template methods instead, just as Append is done.
// NOTE: you can separate it to own header - it will make testing easier.
template<typename T, typename S>
Base::NullValue<T> setFromString(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<T> ret;
    return ret;
  }
  else
  {
    S s;
    r.to(s);
    T data( T::from_string(s) );
    // TODO: memory leak here.
    Base::NullValue<T> ret( data );
    return ret;
  }
}

// TODO: make this partial specialization of set() method.
Base::NullValue<Timestamp> set(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<Timestamp> ret;
    return ret;
  }
  else
  {
    string s;
    r.to(s);
    // TODO: memory leak here
    Base::NullValue<Timestamp> ret( new Timestamp( timestampFromString(s) ) );
    return ret;
  }
}

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
// check if reading values are NULL or not and create objects in proper way !!
// write tests !!
Persistency::AlertPtrNN EntryReader::readAlert(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alerts WHERE id = " << alertID << ";";
  const result r = execSQL(t_, ss);

  string create_time;
  int idSeverity;

  r[0]["create_time"].to(create_time);
  r[0]["id_severity"].to(idSeverity);

  AlertPtrNN alert( new Alert(fromSQLResult<string>(r[0]["name"]),
                              getAnalyzers( alertID ),
                              set(r[0]["detect_time"]).get(),
                              timestampFromString( create_time ),
                              Severity( severityFromInt( fromSQLResult<int>(r[0]["id_severity"]) ) ),
                              Certainty( fromSQLResult<double>(r[0]["certanity"]) ),
                              fromSQLResult<string>(r[0]["description"]),
                              getSourceHosts( alertID ),
                              getTargetHosts( alertID ) ) );
  return alert;
}

Persistency::MetaAlertPtrNN EntryReader::readMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);
  string name, createTime, lastUpdateTime;
  double severityDelta, certaintyDelta;
  r[0]["name"].to(name);
  trim(name);
  r[0]["severity_delta"].to(severityDelta);
  r[0]["certanity_delta"].to(certaintyDelta);
  r[0]["create_time"].to(createTime);

  MetaAlertPtrNN malert( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name),
                                          severityDelta,
                                          certaintyDelta,
                                          getReferenceURL( set<DataBaseID, DataBaseID>(r[0]["id_ref"]).get() ),
                                          timestampFromString( createTime)) );
  return malert;
}

// TODO: tests
AnalyzerPtrNN EntryReader::getAnalyzer(DataBaseID anlzID)
{
  stringstream sa;
  sa << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  const result ra = execSQL(t_, sa);

  AnalyzerPtrNN anlz(new Analyzer( Analyzer::Name( fromSQLResult<string>(ra[0]["name"]) ),
                                   set<Analyzer::Version, string>(ra[0]["version"] ).get(),
                                   set<Analyzer::OS, string>(ra[0]["os"] ).get(),
                                   setFromString<Analyzer::IP, string>(ra[0]["ip"] ).get() ));
  return anlz;
}

Alert::SourceAnalyzers EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  const result r = execSQL(t_, ss);

  Alert::SourceAnalyzers analyzers( getAnalyzer( fromSQLResult<DataBaseID>(r[0]["id_analyzer"]) ) );

  for(size_t i=0; i<r.size(); ++i)
    analyzers.push_back( getAnalyzer( fromSQLResult<DataBaseID>(r[i]["id_analyzer"]) ) );
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
    DataBaseID idHost, idRefURL;
    r[i]["id_host"].to(idHost);
    r[i]["id_ref"].to(idRefURL);
    hosts.push_back( getHost(idHost, &idRefURL) );
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

  string ip, mask, os, name;

  r[0]["ip"].to(ip);
  const Persistency::Host::IP hostIP(Persistency::Host::IP::from_string(ip) );

  HostPtr host(new Persistency::Host(hostIP,
                                     setFromString<Persistency::Host::Netmask, string>(r[0]["mask"]).get(),
                                     *set<Persistency::Host::OperatingSystem, string>(r[0]["os"]).get(),    // TODO: getNotNULL()?
                                     getReferenceURL(refID),
                                     getReportedServices( hostID ),
                                     getReportedProcesses( hostID ),
                                     *set<Persistency::Host::Name, string>(r[0]["name"]).get() ) );     // TODO: getNotNULL()?
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

  string                     name;
  Persistency::Service::Port port;

  r[0]["name"].to(name);
  r[0]["port"].to(port);

  const Persistency::Service::Name serviceName(name);
  const Persistency::Service::Port servicePort(port);

  Persistency::ServicePtrNN service(new Persistency::Service(serviceName,
                                                           servicePort,
                                                           *set<Persistency::Service::Protocol, string>(r[0]["protocol"]).get(),
                                                           getReferenceURL( refID )));
  return service;
}

ProcessPtr EntryReader::getProcess(DataBaseID procID, DataBaseID *refID)
{
  //TODO tests
  stringstream ss;
  ss << "SELECT * FROM processes WHERE id = " << procID << ";";
  const result r = execSQL(t_, ss);

  string name, md5;
  // TODO: consider using NullValue class for this.
  r[0]["name"].to(name);
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

  string username, arguments;
  // TODO: SEGV when no entries returned!
  rr[0]["username"].to(username);
  rr[0]["arguments"].to(arguments);

  const Process::Name     procName(name);
  const Process::Username procUsername(username);

  Persistency::ProcessPtr process( new Process(*set<Process::Path, string>(r[0]["path"]).get(), // TODO: what if path is NULL?
                                                procName,
                                                procMD5,
                                                set<pid_t, int>(rr[0]["pid"]).get(),
                                                set<int, int>(rr[0]["uid"]).get(),
                                                procUsername,
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
  // TODO: segv if not entries returned.
  // throw exception if size != 1

  const ReferenceURL::Name refName( fromSQLResult<string>(r[0]["name"]) );
  const ReferenceURL::URL  refURL( fromSQLResult<string>(r[0]["url"]) );

  refURLPtr = ReferenceURLPtr( new ReferenceURL(refName, refURL) );
  return refURLPtr;
}

double EntryReader::getSeverityDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT severity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);
  // TODO: SEGV if no entruies returned
  return fromSQLResult<double>(r[0]["severity_delta"]);
}
double EntryReader::getCertaintyDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT caertanity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  const result r = execSQL(t_, ss);

  // TODO: SEGV if no entries returned.
  return fromSQLResult<double>(r[0]["certanity_delta"]);
}

Persistency::AlertPtrNN EntryReader::getLeaf(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = execSQL(t_, ss);
  // TODO: SEGV...
  return Persistency::AlertPtrNN( readAlert( fromSQLResult<DataBaseID>(r[0]["id_alert"]) ) );
}

vector<DataBaseID> EntryReader::readMetaAlertChildren(DataBaseID malertID)
{
  vector<DataBaseID> childrenIDs;
  stringstream ss;
  ss << "SELECT id_child FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  const result r = execSQL(t_, ss);

  for(size_t i=0; i<r.size(); ++i)
    childrenIDs.push_back( fromSQLResult<DataBaseID>(r[i]["id_child"]) );
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
    malertsInUse.push_back( fromSQLResult<DataBaseID>(r[i]["id_meta_alert"]) );
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
    malertsBetween.push_back( fromSQLResult<DataBaseID>(r[i]["id"]) );
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
    roots.push_back( fromSQLResult<DataBaseID>(r[i]["id_node"]) );
  }
  return roots;
}

DataBaseID EntryReader::getAlertIDAssociatedWithMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  const result r = execSQL(t_, ss);
  // TODO: first check if result has any entries at all. btw: in fact it should
  //       be checked if we have EXACTLY one result.
  return fromSQLResult<DataBaseID>(r[0]["id_alert"]);
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
