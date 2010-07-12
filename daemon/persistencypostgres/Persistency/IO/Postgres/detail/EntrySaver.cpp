/*
 * EntrySaver.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/ReaderHelper.hpp"
#include "Persistency/IO/Postgres/SQLHelper.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/Appender.hpp"

using namespace std;
using namespace pqxx;

// this is helper macro for calling f-cjtion that saves line number and calls given sql statement (with log)
#define SQL(sql,log) SQLHelper(__FILE__, __LINE__, (sql), (log))

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
template <typename T>
void addToSelect(std::stringstream &ss, const T *ptr)
{
  if(ptr!=NULL)
  {
    ss << " = ";
    Appender::append(ss, ptr->get() );
  }
  else
    ss << " IS NULL";
}

template <>
void addToSelect<Persistency::Analyzer::IP>(std::stringstream &ss, const Persistency::Analyzer::IP *ptr)
{
  if(ptr!=NULL)
  {
    ss << " = ";
    Appender::append(ss, ptr->to_string().c_str() );
  }
  else
    ss << " IS NULL";
}
} //unnamed namespace

EntrySaver::EntrySaver(Transaction &t, DBHandler &dbh):
  log_("persistency.io.postgres.detail.entrysaver"),
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
  const result r=SQL(sql, log_).exec(t_);
  assert( r.size()==1 && "unable to read current sequence number" );

  DataBaseID id;
  r[0]["id"].to(id);
  return id;
}

DataBaseID EntrySaver::getSeverityID(const Alert &a)
{
  // get direct mapping form enum to 'level' column.
  const int level=a.getSeverity().getLevel().toInt();
  assert(level >= 0 && level <= 6);
  // prepare SQL query
  stringstream ss;
  ss<<"SELECT id FROM severities WHERE level="<<level;
  // execute it
  const result r=SQL( ss.str(), log_ ).exec(t_);
  if( r.size()!=1 )
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str() );
  // return read value as a number
  return ReaderHelper<DataBaseID>::readAsNotNull(r[0]["id"]);
}

void EntrySaver::addReferenceURL(std::stringstream &ss, const ReferenceURL *url)
{
  if( url!=NULL )
  {
    const DataBaseID urlID=saveReferenceURL( *url );
    ss << urlID;
  }
  else
    ss << "NULL";
}

Base::NullValue<DataBaseID> EntrySaver::isAnalyzerInDataBase(const Analyzer &a)
{
  DataBaseID id;
  stringstream ss;
  ss << "SELECT * FROM analyzers WHERE name = ";
  Appender::append(ss, a.getName().get() );
  ss << " AND version ";
  addToSelect(ss, &a.getVersion() );
  ss << " AND os";
  addToSelect(ss, &a.getOperatingSystem() );
  ss << " AND ip";
  addToSelect(ss, a.getIP() );
  ss << " AND sys_id=";
  Appender::append(ss, a.getID().get() );
  ss << ";";
  const result r=SQL( ss.str(), log_ ).exec(t_);
  if( r.empty() )
  {
    LOGMSG_DEBUG_S(log_)<<"analyzer '"<<a.getName().get()<<"' not found in data base";
    return Base::NullValue<DataBaseID>();
  }
  r[0]["id"].to(id);
  LOGMSG_DEBUG_S(log_)<<"analyzer '"<<a.getName().get()<<"' found in data base - id=="<<id;
  return Base::NullValue<DataBaseID>( id );
}

DataBaseID EntrySaver::saveProcessData(const Process &p)
{
  stringstream ss;
  ss << "INSERT INTO procs(path, name, md5) VALUES (";
  Appender::append(ss, p.getPath().get() );
  ss << ",";
  Appender::append(ss, p.getName().get() );
  ss << ",";
  Appender::append(ss, (p.getMD5())?p.getMD5()->get():NULL);
  ss << ");";
  // insert object to data base.
  SQL( ss.str(), log_ ).exec(t_);

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
  addReferenceURL(ss, p.getReferenceURL() );
  ss << ");";
  // insert object to data base.
  SQL( ss.str(), log_ ).exec(t_);

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
  SQL( ss.str(), log_ ).exec(t_);

  return getID("reference_urls_id_seq");
}

DataBaseID EntrySaver::saveHostData(const Persistency::Host &h)
{
  stringstream ss;
  ss << "INSERT INTO hosts(ip, mask, os, name) VALUES (";
  Appender::append(ss, h.getIP().to_string() );
  ss << ",";
  Appender::append(ss, h.getNetmask()?h.getNetmask()->to_string().c_str():NULL );
  ss << ",";
  Appender::append(ss, h.getOperatingSystem().get() );
  ss << ",";
  Appender::append(ss, h.getName().get() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);

  return getID("hosts_id_seq");
}

DataBaseID EntrySaver::saveReportedHostData(DataBaseID               alertID,
                                            DataBaseID               hostID,
                                            const char               *role,
                                            const Persistency::Host &h)
{
  stringstream ss;
  ss << "INSERT INTO reported_hosts(id_alert, id_host, role, id_ref) VALUES (";
  ss << alertID << ",";
  ss << hostID << ",";
  assert(strcmp(role, "src") == 0 || strcmp(role, "dst") == 0);
  Appender::append(ss, role);
  ss << ",";
  addReferenceURL(ss, h.getReferenceURL() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
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

DataBaseID EntrySaver::saveAlert(const Persistency::Alert &a)
{
  stringstream ss;
  ss << "INSERT INTO alerts(name, detect_time, create_time, id_severity, certanity, description) VALUES (";
  Appender::append(ss, a.getName().get() );
  ss << ",";
  Appender::append(ss, a.getDetectionTime()?a.getDetectionTime():NULL);
  ss << ",";
  Appender::append(ss, a.getCreationTime() );
  ss << ",";
  const DataBaseID sevID = getSeverityID(a);
  Appender::append(ss, sevID);
  ss << ",";
  Appender::append(ss, a.getCertainty().get() );
  ss << ",";
  Appender::append(ss, a.getDescription() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
  return getID("alerts_id_seq");
}

DataBaseID EntrySaver::saveAnalyzer(const Analyzer &a)
{
  Base::NullValue<DataBaseID> id = isAnalyzerInDataBase(a);
  if( id.get()!=NULL )
    return *id.get();
  // if not present, add it
  stringstream ss;
  ss << "INSERT INTO analyzers(sys_id, name, version, os, ip) VALUES (";
  Appender::append(ss, a.getID().get() );
  ss << ",";
  Appender::append(ss, a.getName().get() );
  ss << ",";
  Appender::append(ss, a.getVersion().get() );
  ss << ",";
  Appender::append(ss, a.getOperatingSystem().get() );
  ss << ",";
  Appender::append(ss, a.getIP()?( a.getIP()->to_string().c_str() ):NULL);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
  return getID("analyzers_id_seq");
}

DataBaseID EntrySaver::saveServiceData(const Service &s)
{
  stringstream ss;
  ss << "INSERT INTO services(name, port, protocol) VALUES (";
  Appender::append(ss, s.getName().get() );
  ss << ",";
  Appender::append(ss, s.getPort().get() );
  ss <<",";
  Appender::append(ss, s.getProtocol().get() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);

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
  addReferenceURL(ss, s.getReferenceURL() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
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
  ss << "INSERT INTO meta_alerts(name, severity_delta, certainty_delta, id_ref, create_time, last_update_time) VALUES (";
  Appender::append(ss, ma.getName().get() );
  ss << ",";
  Appender::append(ss, ma.getSeverityDelta() );
  ss << ",";
  Appender::append(ss, ma.getCertaintyDelta() );
  ss << ",";
  addReferenceURL(ss, ma.getReferenceURL() );
  ss << ",";
  Appender::append(ss, ma.getCreateTime() );
  ss << ",";
  Appender::append(ss, "now()");
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);

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
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::saveAlertToAnalyzers(DataBaseID alertID, DataBaseID anlzID)
{
  stringstream ss;
  ss << "INSERT INTO alert_analyzers(id_alert, id_analyzer) VALUES (";
  Appender::append(ss, alertID);
  ss << ",";
  Appender::append(ss, anlzID);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}


void EntrySaver::saveMetaAlertsTree(DataBaseID nodeID, DataBaseID childID)
{
  stringstream ss;
  ss << "INSERT INTO meta_alerts_tree(id_node, id_child) VALUES (";
  Appender::append(ss, nodeID);
  ss << ",";
  Appender::append(ss, childID);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::markMetaAlertAsUsed(DataBaseID malertID)
{
  stringstream ss;
  ss << "INSERT INTO meta_alerts_in_use(id_meta_alert) VALUES (" << malertID << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::markMetaAlertAsUnused(DataBaseID malertID)
{
  removeMetaAlertFromTriggered(malertID);
  stringstream ss;
  ss << "DELETE FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::markMetaAlertAsTriggered(DataBaseID malertID, const std::string &name)
{
  stringstream ss;
  ss << "INSERT INTO meta_alerts_already_triggered(id_meta_alert_in_use, trigger_name) VALUES(";
  Appender::append(ss, malertID);
  ss << ",";
  Appender::append(ss, name);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::removeMetaAlertFromTriggered(DataBaseID malertID)
{
  stringstream ss;
  ss << "DELETE FROM  meta_alerts_already_triggered WHERE id_meta_alert_in_use = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::updateSeverityDelta(DataBaseID malertID, double severityDelta)
{
  stringstream ss;
  ss << "UPDATE meta_alerts SET severity_delta = severity_delta + ";
  Appender::append(ss, severityDelta);
  ss << ", last_update_time = now() ";
  ss << " WHERE id = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::updateCertaintyDelta(DataBaseID malertID, double certanityDelta)
{
  stringstream ss;
  ss << "UPDATE meta_alerts SET certainty_delta = certainty_delta + ";
  Appender::append(ss, certanityDelta);
  ss << ", last_update_time = now() ";
  ss << " WHERE id = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

bool EntrySaver::isHostNameNull(DataBaseID hostID)
{
  stringstream ss;
  ss << "SELECT name FROM hosts WHERE id = " << hostID << ";";
  const result r=SQL( ss.str(), log_ ).exec(t_);
  if(r.size() != 1)
    throw ExceptionNoEntries(SYSTEM_SAVE_LOCATION, ss.str());
  return r[0]["name"].is_null();
}

void EntrySaver::setHostName(DataBaseID hostID, const Persistency::Host::Name &name)
{
  if(isHostNameNull(hostID) == false)
    throw ExceptionHostNameAlreadySaved(SYSTEM_SAVE_LOCATION);
  stringstream ss;
  ss << "UPDATE hosts SET name = ";
  Appender::append(ss, name.get());
  ss << " WHERE id = " << hostID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::saveConfigParameter(const DynamicConfig::Owner &owner,
                                     const DynamicConfig::Key   &key,
                                     const DynamicConfig::Value &value)
{
  // first, delete given key if entry's present
  {
    stringstream ss;
    ss << "DELETE FROM config WHERE owner = ";
    Appender::append(ss, owner.get());
    ss << " AND key = ";
    Appender::append(ss, key.get());
    SQL( ss.str().c_str(), log_ ).exec(t_);
  }
  // now add entry
  {
    stringstream ss;
    ss << "INSERT INTO config (owner, key, value) VALUES (";
    Appender::append(ss, owner.get());
    ss << ",";
    Appender::append(ss, key.get());
    ss << ",";
    Appender::append(ss, value.get());
    ss << ")";
    SQL( ss.str().c_str(), log_ ).exec(t_);
  }
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
