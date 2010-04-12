/*
 * EntryReader.cpp
 *
 */
#include <sstream>
#include <ctime>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/detail/append.hpp"

using namespace std;
using namespace pqxx;

using boost::lexical_cast;
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

SeverityLevel fromInt(int level)
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
  // temporary solution
  return SeverityLevel::CRITICAL;
  // TODO: throw exception when value of severity level is wrong
}

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
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  string name, description, detect_time, create_time;
  int idSeverity;
  double certainty;

  r[0]["name"].to(name);
  r[0]["description"].to(description);
  r[0]["create_time"].to(create_time);
  r[0]["id_severity"].to(idSeverity);
  r[0]["certanity"].to(certainty);
  // TODO: consider using NullValue class for this.
  Timestamp                *alertDetect = NULL;
  if( !r[0]["detect_time"].is_null() )
  {
    r[0]["detect_time"].to(detect_time);
    //TODO: smart pointer
    alertDetect = new Timestamp( lexical_cast<time_t>(detect_time) );
  }

  const Persistency::Alert::Name alertName(name);
  const Timestamp                alertCreate( lexical_cast<time_t>( create_time ) );
  const Severity                 alertSeverity( fromInt(idSeverity) );
  const Certainty                alertCertainty(certainty);
  const string                   alertDescription(description);

  AlertPtrNN alert( new Alert(alertName,
                              getAnalyzers( alertID ),
                              alertDetect,
                              alertCreate,
                              alertSeverity,
                              alertCertainty,
                              alertDescription,
                              getSourceHosts( alertID ),
                              getTargetHosts( alertID ) ) );
  return alert;
}

Persistency::MetaAlertPtrNN EntryReader::readMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  string name, createTime, lastUpdateTime;
  double severityDelta, certaintyDelta;
  DataBaseID *refID = NULL, id;
  r[0]["name"].to(name);
  trim(name);
  r[0]["severity_delta"].to(severityDelta);
  r[0]["certanity_delta"].to(certaintyDelta);
  r[0]["create_time"].to(createTime);
  // TODO: consider using NullValue class for this.
  if( !r[0]["id_ref"].is_null() )
  {
    r[0]["id_ref"].to(id);
    //TODO smart pointer
    refID = new DataBaseID(id);
  }
  const Persistency::MetaAlert::Name malertName(name);
  Timestamp                          malertCreate( lexical_cast<time_t>(createTime) );

  MetaAlertPtrNN malert( new Persistency::MetaAlert( malertName,
                                          severityDelta,
                                          certaintyDelta,
                                          getReferenceURL( refID  ),
                                          malertCreate ) );
  return malert;
}

// TODO: tests
AnalyzerPtrNN EntryReader::getAnalyzer(DataBaseID anlzID)
{
  stringstream sa;
  sa << "SELECT * FROM analyzers WHERE id = " << anlzID << ";";
  result ra = t_.getAPI<TransactionAPI>().exec(sa);

  string name, version, os, ip;
  ra[0]["name"].to(name);

  Analyzer::Version *anlzVersion = NULL;
  // TODO: consider using NullValue class for this.
  if( !ra[0]["version"].is_null() )
  {
    ra[0]["version"].to(version);
    anlzVersion =  new Analyzer::Version(version) ;
    //TODO smart pointer
  }

  // TODO: consider using NullValue class for this.
  Analyzer::OS *anlzOS = NULL;
  if( !ra[0]["os"].is_null() )
  {
    ra[0]["os"].to(os);
    anlzOS = new Analyzer::OS(os);
    //TODO smart pointer
  }

  // TODO: consider using NullValue class for this.
  Analyzer::IP *anlzIP = NULL;
  if( !ra[0]["ip"].is_null() )
  {
    ra[0]["ip"].to(ip);
    anlzIP = new Analyzer::IP( Analyzer::IPv4::from_string(ip) );
    //TODO smart pointer
  }

  const Analyzer::Name    anlzName(name);

  AnalyzerPtrNN anlz(new Analyzer( anlzName,
                                   anlzVersion,
                                   anlzOS,
                                   anlzIP ));
  return anlz;
}

Alert::SourceAnalyzers EntryReader::getAnalyzers(DataBaseID alertID)
{
  stringstream ss;
  ss << "SELECT * FROM alert_analyzers WHERE id_alert = " << alertID <<";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  DataBaseID id;
  r[0]["id_analyzer"].to(id);
  Alert::SourceAnalyzers analyzers( getAnalyzer(id) );

  for(unsigned int i=0; i<r.size(); ++i)
  {
    r[i]["id_analyzer"].to(id);

    analyzers.push_back( getAnalyzer(id) );
  }
  return analyzers;
}

Alert::ReportedHosts EntryReader::getReporteHosts(DataBaseID alertID, std::string hostType)
{
  stringstream ss;
  ss << "SELECT * FROM reported_hosts WHERE id_alert = "<< alertID <<" AND role = ";
  Appender::append(ss, hostType);
  ss << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  Alert::ReportedHosts hosts;
  for(unsigned int i=0; i<r.size(); ++i)
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
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  string ip, mask, os, name;

  r[0]["ip"].to(ip);

  // TODO: consider using NullValue class for this.
  Persistency::Host::Name            hostName;
  if( !r[0]["name"].is_null() )
  {
    r[0]["name"].to(name);
    hostName = Persistency::Host::Name(name);
  }

  // TODO: consider using NullValue class for this.
  Persistency::Host::OperatingSystem hostOS;
  if( !r[0]["os"].is_null() )
  {
    r[0]["os"].to(os);
    hostOS = Persistency::Host::OperatingSystem(os);
  }

  // TODO: consider using NullValue class for this.
  Persistency::Host::Netmask         hostIP;
  if( !r[0]["mask"].is_null() )
  {
    r[0]["mask"].to(mask);
    hostIP = Persistency::Host::Netmask( Persistency::Host::Netmask::from_string(ip) );
  }

  const Persistency::Host::IP              hostNetmask(
                    Persistency::Host::IP::from_string(mask) );

  // TODO: you do not know if given values are set or not. i'd suggest
  //       reversing logic - if they are NULL (but shouldn't) throw an
  //       exception. notice that this code is generic, therofor it can be
  //       implemented as a template.
  HostPtr host(new Persistency::Host(hostIP,
                                     &hostNetmask,
                                     hostOS,
                                     getReferenceURL(refID),
                                     getReportedServices( hostID ),
                                     getReportedProcesses( hostID ),
                                     hostName) );
  // add host to cache
  dbh_.getIDCache()->add(host , hostID);
  return host;
}

Persistency::Host::ReportedServices EntryReader::getReportedServices(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT * FROM reported_services WHERE id_reported_host = " << hostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  Persistency::Host::ReportedServices services;
  DataBaseID idService, idRef;
  for(unsigned int i = 0;i < r.size(); ++i)
  {
    r[i]["id_service"].to(idService);
    r[i]["id_ref"].to(idRef);
    services.push_back( getService(idService, &idRef) );
  }
  return services;
  //TODO
}

Persistency::Host::ReportedProcesses EntryReader::getReportedProcesses(DataBaseID hostID)
{
  //TODO
  stringstream ss;
  ss << "SELECT * FROM reported_procs WHERE id_reported_host = " << hostID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  Persistency::Host::ReportedProcesses processes;
  DataBaseID idProcess, idRef;
  for(unsigned int i = 0; i<r.size(); ++i)
  {
    r[i]["id_proc"].to(idProcess);
    r[i]["id_ref"].to(idRef);
    processes.push_back( getProcess(idProcess, &idRef) );
  }
  return processes;
}

Persistency::ServicePtr EntryReader::getService(DataBaseID servID, DataBaseID *refID)
{
  //TODO
  stringstream ss;
  ss << "SELECT * FROM services WHERE id = " << servID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  string  name;
  string  protocol;
  // TODO: use Persistency::Service::Port as a type instead of manually specifing it.
  int16_t port;

  r[0]["name"].to(name);
  r[0]["protocol"].to(protocol);
  Persistency::Service::Protocol serviceProtocol;
  if( !r[0]["port"].is_null() )
  {
    r[0]["port"].to(port);
    serviceProtocol = Persistency::Service::Protocol(protocol);
  }

  const Persistency::Service::Name     serviceName(name);
  const Persistency::Service::Port     servicePort(port);

  Persistency::ServicePtr service(new Persistency::Service(serviceName,
                                                           servicePort,
                                                           serviceProtocol,
                                                           getReferenceURL( refID )));
  return service;
}

ProcessPtr EntryReader::getProcess(DataBaseID procID, DataBaseID *refID)
{
  //TODO tests
  stringstream ss;
  ss << "SELECT * FROM processes WHERE id = " << procID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  string path, name, md5;
  Process::Path     procPath;
  if( !r[0]["path"].is_null() )
  {
    r[0]["path"].to(path);
    procPath = Process::Path(path);
  }

  // TODO: consider using NullValue class for this.
  r[0]["name"].to(name);
  MD5Sum *procMD5 = NULL;
  if( !r[0]["md5"].is_null() )
  {
    r[0]["md5"].to(md5);
    // TODO: SEGV here - procMD5 is NULL in this context
    *procMD5 = MD5Sum( MD5Sum::createFromString(md5.c_str()) );
  }

  stringstream sr;
  sr << "SELECT * FROM reported_procs WHERE id_proc = " << procID << ";";
  result rr = t_.getAPI<TransactionAPI>().exec(sr);

  string username, arguments;
  int pid, uid;
  rr[0]["pid"].to(pid);
  rr[0]["uid"].to(uid);
  rr[0]["username"].to(username);
  rr[0]["arguments"].to(arguments);

  const Process::Name     procName(name);
  const pid_t             procPid(pid);
  const Process::Username procUsername(username);

  // TODO: what if procPid and/or uid are NULLs in data base?
  Persistency::ProcessPtr process( new Process( procPath,
                                                procName,
                                                procMD5,
                                               &procPid,
                                               &uid,
                                                procUsername,
                                                arguments.c_str(),
                                                getReferenceURL( refID )) );
  return process;
}

ReferenceURLPtr EntryReader::getReferenceURL(DataBaseID *refID)
{
  ReferenceURLPtr refURLPtr;
  if(refID == NULL)
    return refURLPtr;
  stringstream ss;
  ss << "SELECT * FROM reference_urls WHERE id = " << *refID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  string name, url;
  r[0]["name"].to(name);
  r[0]["url"].to(url);

  const ReferenceURL::Name refName(name);
  const ReferenceURL::URL  refURL(url);

  refURLPtr = ReferenceURLPtr( new ReferenceURL(refName, refURL) );
  return refURLPtr;
}

double EntryReader::getSeverityDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT severity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  double severity;
  r[0]["severity_delta"].to(severity);
  return severity;
}
double EntryReader::getCertaintyDelta(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT caertanity_delta FROM meta_alerts WHERE id = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  double certainty;
  r[0]["certanity_delta"].to(certainty);
  return certainty;
}

Persistency::AlertPtrNN EntryReader::getLeaf(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  DataBaseID idAlert;
  r[0]["id_alert"].to(idAlert);
  return Persistency::AlertPtrNN( readAlert(idAlert) );
  //dbh_.getIDCache()->add(readAlert(idAlert) , idAlert);
  //dbh_.getIDCache()->add(readMetaAlert(idMetaAlert) , idMetaAlert);
}

vector<DataBaseID> EntryReader::readMetaAlertChildren(DataBaseID malertID)
{
  vector<DataBaseID> childrenIDs;
  stringstream ss;
  ss << "SELECT id_child FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  for(unsigned int i=0; i<r.size(); ++i)
  {
    DataBaseID idChild;
    r[i]["id_child"].to(idChild);
    childrenIDs.push_back(idChild);
  }
  return childrenIDs;
}

vector<DataBaseID> EntryReader::readIDsMalertsInUse()
{
  vector<DataBaseID> malertsInUse;
  stringstream ss;
  ss << "SELECT id_meta_alert FROM meta_alerts_in_use;";
  // TODO: result should be const
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  malertsInUse.reserve( r.size() );
  // TODO: use size_t instead of unsigned int
  for(unsigned int i=0; i<r.size(); ++i)
  {
    // TODO: add assertion that id_meta_alert is not NULL - DB schema should
    //       enforce this, but it's better to be safe then sorry.
    DataBaseID malertID;
    r[i]["id_meta_alert"].to(malertID);
    malertsInUse.push_back(malertID);
  }
  return malertsInUse;
}

vector<DataBaseID> EntryReader::readIDsMalertsBetween(const Timestamp &from, const Timestamp &to)
{
  vector<DataBaseID> malertsBetween;
  stringstream ss;
  //TODO: test this query
  ss << "SELECT id FROM meta_alerts WHERE "
     // TODO: 'from' and 'to' should be inserted here using proper query (append()
     //       call will be handy here).
     << from << " <= create_time AND create_time <=" << to << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  for(unsigned int i=0; i<r.size(); ++i)
  {
    DataBaseID malertID;
    r[i]["id"].to(malertID);
    malertsBetween.push_back(malertID);
  }
  return malertsBetween;
}

std::vector<DataBaseID> EntryReader::readRoots()
{
  vector<DataBaseID> roots;
  // TODO: when you know whole query a'priori do not put it additionaly in
  //       stringstream - it takes extra time and memory. better user simple
  //       const char * for this, or if query is trivial (i.e. short like
  //       'drop table x') write it stright in exec() call
  //        [ btw: nice trick with temporary table! :) ]
  stringstream ss;
  ss << "SELECT id_node, id_child INTO TEMP TABLE tmp FROM meta_alerts_tree"
        " INNER JOIN meta_alerts_in_use ON(meta_alerts_tree.id_node=meta_alerts_in_use.id_meta_alert);";
  t_.getAPI<TransactionAPI>().exec(ss);
  ss.str("");
  ss << "SELECT DISTINCT T.id_node FROM tmp T WHERE NOT EXISTS( SELECT 1 FROM tmp S WHERE T.id_node=S.id_child );";

  result r = t_.getAPI<TransactionAPI>().exec(ss);
  ss.str("");
  ss << "DROP TABLE tmp;";
  t_.getAPI<TransactionAPI>().exec(ss);
  // TODO: use size_t here
  for(unsigned int i=0; i<r.size(); ++i)
  {
    // TODO: use assert to ensure value is not null
    DataBaseID nodeID;
    r[i]["id_node"].to(nodeID);
    roots.push_back(nodeID);
  }
  return roots;
}

DataBaseID EntryReader::getAlertIDAssociatedWithMetaAlert(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert = " << malertID << ";";
  // TODO: result should be const
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  DataBaseID idAlert;
  // TODO: first check if result has any entries at all. btw: in fact it should
  //       be checked if we have EXACTLY one result.
  r[0]["id_alert"].to(idAlert);
  return idAlert;
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
