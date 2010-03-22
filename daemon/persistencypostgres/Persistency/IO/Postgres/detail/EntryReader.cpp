/*
 * EntryReader.cpp
 *
 */
#include <sstream>
#include <cassert>
#include <boost/algorithm/string.hpp>

#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/detail/append.hpp"

using namespace std;
using namespace pqxx;

using Persistency::IO::Transaction;
using boost::posix_time::time_from_string;
using boost::algorithm::trim;

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
  Timestamp                *alertDetect = NULL;
  if( !r[0]["detect_time"].is_null() )
  {
    r[0]["detect_time"].to(detect_time);
    alertDetect = new Timestamp( time_from_string(detect_time) );
  }

  const Persistency::Alert::Name alertName(name);
  const Timestamp                alertCreate( time_from_string( create_time ) );
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
  //if(alertDetect) delete alertDetect;
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
  if( !r[0]["id_ref"].is_null() )
  {
    r[0]["id_ref"].to(id);
    refID = new DataBaseID(id);
  }
  const Persistency::MetaAlert::Name malertName(name);
  Timestamp                          malertCreate( time_from_string( createTime) );

  //TODO: check if id_ref is NULL
  MetaAlertPtrNN malert( new Persistency::MetaAlert( malertName,
                                          severityDelta,
                                          certaintyDelta,
                                          getReferenceURL( refID  ),
                                          malertCreate ) );
  if(refID) delete refID;
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
  if( !ra[0]["version"].is_null() )
  {
    ra[0]["version"].to(version);
    anlzVersion =  new Analyzer::Version(version) ;
  }

  Analyzer::OS *anlzOS = NULL;
  if( !ra[0]["os"].is_null() )
  {
    ra[0]["os"].to(os);
    anlzOS = new Analyzer::OS(os);
  }

  Analyzer::IP *anlzIP = NULL;
  if( !ra[0]["ip"].is_null() )
  {
    ra[0]["ip"].to(ip);
    anlzIP = new Analyzer::IP( Analyzer::IPv4::from_string(ip) );
  }

  const Analyzer::Name    anlzName(name);

  AnalyzerPtrNN anlz(new Analyzer( anlzName,
                                   anlzVersion,
                                   anlzOS,
                                   anlzIP ));
  if(anlzOS)      delete anlzOS;
  if(anlzVersion) delete anlzVersion;
  if(anlzIP)      delete anlzIP;
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

  Persistency::Host::Name            hostName;
  if( !r[0]["name"].is_null() )
  {
    r[0]["name"].to(name);
    hostName = Persistency::Host::Name(name);
  }

  Persistency::Host::OperatingSystem hostOS;
  if( !r[0]["os"].is_null() )
  {
    r[0]["os"].to(os);
    hostOS = Persistency::Host::OperatingSystem(os);
  }

  Persistency::Host::Netmask         hostIP;
  if( !r[0]["mask"].is_null() )
  {
    r[0]["mask"].to(mask);
    hostIP = Persistency::Host::Netmask( Persistency::Host::Netmask::from_string(ip) );
  }

  const Persistency::Host::IP              hostNetmask(
                    Persistency::Host::IP::from_string(mask) );

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

  string name, protocol;
  int port;

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

  r[0]["name"].to(name);
  MD5Sum *procMD5 = NULL;
  if( !r[0]["md5"].is_null() )
  {
    r[0]["md5"].to(md5);
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

  Persistency::ProcessPtr process( new Process(procPath,
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

size_t EntryReader::getChildrenIDs(DataBaseID malertID)
{
  stringstream ss;
  ss << "SELECT * FROM meta_alerts_tree WHERE id_node = " << malertID << ";";
  result r = t_.getAPI<TransactionAPI>().exec(ss);

  return r.size();
}

void EntryReader::getLeafs(leafsMap &leafs)
{
  stringstream ss;
  ss << "SELECT * FROM alert_to_meta_alert_map INNER JOIN meta_alerts_in_use ON (alert_to_meta_alert_map.id_meta_alert = meta_alerts_in_use.id_meta_alert);";
  result r = t_.getAPI<TransactionAPI>().exec(ss);
  for(unsigned int i=0; i<r.size(); ++i)
  {
    int idAlert;
    int idMetaAlert;
    r[i]["id_alert"].to(idAlert);
    r[i]["id_meta_alert"].to(idMetaAlert);
    dbh_.getIDCache()->add(readAlert(idAlert) , idAlert);
    dbh_.getIDCache()->add(readMetaAlert(idMetaAlert) , idMetaAlert);
    leafs.insert( pair<DataBaseID, Persistency::AlertPtrNN>(idMetaAlert, readAlert(idAlert) ) );
  }
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
