/*
 * EntrySaver.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/append.hpp"

using namespace std;
using namespace pqxx;
using boost::posix_time::to_simple_string;

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

EntrySaver::EntrySaver(Transaction &t, DBHandler &dbh):
  dbh_(dbh),
  t_(t)
{
}

DataBaseID EntrySaver::saveProcess(DataBaseID reportedHostID, const Process &p)
{
  const DataBaseID procID=saveProcessData(p);
  return saveReportedProcessData(reportedHostID, procID, p);
}

DataBaseID EntrySaver::getID(const std::string &seqName)
{
  assert( seqName==pqxx::sqlesc(seqName) && "invalid sequence name" );

  const std::string sql="SELECT currval('" + seqName + "') as id;";
  result r=t_.getAPI<Postgres::TransactionAPI>().exec(sql);
  assert( r.size()==1 && "unable to read current sequence number" );

  DataBaseID id;
  r[0]["id"].to(id);
  return id;
}

DataBaseID EntrySaver::getSeverityID(const Alert &a)
{
  // TODO: add assert here
  return a.getSeverity().getLevel().toInt();
}

DataBaseID EntrySaver::saveProcessData(const Process &p)
{
  stringstream ss;
  ss << "INSERT INTO procs(path, name, md5) VALUES (";
  ss << "'" << pqxx::sqlesc( p.getPath().get() ) << "',";
  ss << "'" << pqxx::sqlesc( p.getName().get() ) << "',";
  Appender::append(ss, (p.getMD5())?p.getMD5()->get():NULL);
  ss << ");";
  // insert object to data base.
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("procs_id_seq");
}

DataBaseID EntrySaver::saveReportedProcessData(DataBaseID     reportedHostID,
                                               DataBaseID     procID,
                                               const Process &p)
{
  stringstream ss;
  ss << "INSERT INTO reported_procs("
        "id_reported_host, id_proc, pid, uid, username, arguments, id_ref"
        ") VALUES (";
  ss << reportedHostID << ",";
  ss << procID << ",";
  Appender::append(ss, p.getPID() );
  ss << ",";
  Appender::append(ss, p.getUID() );
  ss << ",";
  Appender::append(ss, p.getUsername().get() );
  ss << ",";
  Appender::append(ss, p.getParameters() );
  ss << ",";
  if( p.getReferenceURL()!=NULL )
  {
    const DataBaseID urlID=saveReferenceURL( *p.getReferenceURL() );
    ss << urlID;
  }
  else
    ss << "NULL";
  ss << ");";
  // insert object to data base.
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("reported_procs_id_seq");
}

DataBaseID EntrySaver::saveReferenceURL(const ReferenceURL &url)
{
  stringstream ss;
  ss << "INSERT INTO reference_urls(name, url) VALUES (";
  Appender::append(ss, url.getName().get() );
  ss << ",";
  Appender::append(ss, url.getURL().get() );
  ss << ");";
  // insert object to data base.
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("reference_urls_id_seq");
}

DataBaseID EntrySaver::saveHostData(const Persistency::Host &h)
{
  stringstream ss;
  ss << "INSERT INTO hosts(ip, mask, os, name) VALUES (";
  Appender::append(ss, h.getIP().to_string() );
  ss << ",";
  Appender::append(ss, h.getNetmask()->to_string() );
  ss << ",";
  Appender::append(ss, h.getOperatingSystem().get() );
  ss << ",";
  Appender::append(ss, (h.getName())?h.getName()->get():NULL);
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("hosts_id_seq");
}

DataBaseID EntrySaver::saveReportedHostData(DataBaseID               alertID,
                                            DataBaseID               hostID,
                                            const std::string        role,
                                            const Persistency::Host &h)
{
  stringstream ss;
  ss << "INSERT INTO reported_hosts(id_alert, id_host, role, id_ref) VALUES (";
  ss << alertID << ",";
  ss << hostID << ",";
  Appender::append(ss, role);
  ss << ",";
  Appender::append(ss, h.getReferenceURL()?saveReferenceURL( *h.getReferenceURL() ):NULL);
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);
  return getID("reported_hosts_id_seq");
}

DataBaseID EntrySaver::saveTargetHost(DataBaseID hostID, DataBaseID alertID, const Persistency::Host &h)
{
  return saveReportedHostData(alertID, hostID, "dst", h);
}

DataBaseID EntrySaver::saveSourceHost(DataBaseID hostID, DataBaseID alertID, const Persistency::Host &h)
{
  return saveReportedHostData(alertID, hostID, "src", h);
}

DataBaseID EntrySaver::saveAlert(DataBaseID AnalyzerID, const Persistency::Alert &a)
{
  stringstream ss;
  ss << "INSERT INTO alerts(name, id_analyzer, detect_time, create_time, id_severity, certanity, description) VALUES (";
  Appender::append(ss, a.getName().get() );
  ss << ",";
  Appender::append(ss, AnalyzerID);
  ss << ",";
  Appender::append(ss, to_simple_string(*a.getDetectionTime() ));
  ss << ",";
  Appender::append(ss, to_simple_string(a.getCreationTime() ) );
  ss << ",";
  const DataBaseID sevID = getSeverityID(a);
  ss << sevID << ",";
  Appender::append(ss, a.getCertainty().get() );
  ss << ",";
  Appender::append(ss, a.getDescription() );
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("alerts_id_seq");
}

DataBaseID EntrySaver::saveAnalyzer(const DataBaseID *HostID, const Analyzer &a)
{
  stringstream ss;
  ss << "SELECT * FROM analyzers WHERE id_host ";
  if(HostID==NULL)
    ss << "IS NULL";
  else
    ss << "= "<< *HostID;
  ss << " and name = ";
  Appender::append(ss, a.getName().get() );
  ss << ";";
  result r=t_.getAPI<Postgres::TransactionAPI>().exec(ss);
  if(r.empty())
  {
    ss.str("");
    ss << "INSERT INTO analyzers(name, id_host) VALUES (";
    Appender::append(ss, a.getName().get() );
    ss << ",";
    if(HostID==NULL)
      ss << "NULL";
    else
      ss << *HostID;
    ss << ");";
    t_.getAPI<Postgres::TransactionAPI>().exec(ss);
    return getID("analyzers_id_seq");
  }
  else
  {
    DataBaseID id;
    r[0]["id"].to(id);
    return id;
  }
}

DataBaseID EntrySaver::saveServiceData(const Service &s)
{
  stringstream ss;
  ss << "INSERT INTO services(name, port, protocol) VALUES (";
  Appender::append(ss, s.getName().get() );
  ss << ",";
  Appender::append(ss, s.getPort() );
  ss <<",";
  Appender::append(ss, s.getProtocol().get() );
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("services_id_seq");

}

void EntrySaver::saveReportedServiceData(DataBaseID     reportedHostID,
                                         DataBaseID     serID,
                                         const Service &s)
{
  stringstream ss;
  ss << "INSERT INTO reported_services(id_reported_host, id_service, id_ref) VALUES (";
  ss << reportedHostID << ",";
  ss << serID << ",";
  Appender::append(ss, s.getReferenceURL()?saveReferenceURL( *s.getReferenceURL() ):NULL);
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);
}

DataBaseID EntrySaver::saveService(DataBaseID reportedHostID, const Service &s)
{
  DataBaseID serID = saveServiceData(s);
  saveReportedServiceData(reportedHostID, serID, s);
  return serID;
}

DataBaseID EntrySaver::saveMetaAlert(const Persistency::MetaAlert &ma)
{
  stringstream ss;
  ss << "INSERT INTO meta_alerts(name, severity_delta, certanity_delta, id_ref, create_time, last_update_time) VALUES (";
  Appender::append(ss, ma.getName().get() );
  ss << ",";
  Appender::append(ss, ma.getSeverityDelta() );
  ss << ",";
  Appender::append(ss, ma.getCertaintyDelta() );
  ss << ",";
  Appender::append(ss, ma.getReferenceURL()?saveReferenceURL( *ma.getReferenceURL() ):NULL);
  ss << ",";
  Appender::append(ss, to_simple_string( ma.getCreateTime() ));
  ss << ",";
  Appender::append(ss, "now()");
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);

  return getID("meta_alerts_id_seq");
}

void EntrySaver::saveAlertToMetaAlertMap(DataBaseID alertID, DataBaseID malertID)
{
  stringstream ss;
  ss << "INSERT INTO alert_to_meta_alert_map(id_alert, id_meta_alert) VALUES(";
  Appender::append(ss, alertID);
  ss << ",";
  Appender::append(ss, malertID);
  ss << ");";
  t_.getAPI<Postgres::TransactionAPI>().exec(ss);
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
