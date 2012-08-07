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
void addEqualityComparison(std::stringstream &ss, Appender &ap, const T *ptr)
{
  if(ptr!=NULL)
  {
    ss << " = ";
    ap.append(ss, ptr );
  }
  else
    ss << " IS NULL";
}

template <typename T>
void addEqualityComparison(std::stringstream &ss, Appender &ap, const T &ptr)
{
  if(ptr.get()!=NULL)
  {
    ss << " = ";
    ap.append(ss, ptr.get() );
  }
  else
    ss << " IS NULL";
}
template <>
void addEqualityComparison<Persistency::Analyzer::IP>(std::stringstream               &ss,
                                                      Appender                        &ap,
                                                      const Persistency::Analyzer::IP *ptr)
{
  if(ptr!=NULL)
  {
    ss << " = ";
    ap.append(ss, ptr->to_string().c_str() );
  }
  else
    ss << " IS NULL";
}
} // unnamed namespace

EntrySaver::EntrySaver(Transaction &t, DBHandle &dbh):
  log_("persistency.io.postgres.detail.entrysaver"),
  dbh_(dbh),
  t_(t)
{
}

DataBaseID EntrySaver::saveProcess(DataBaseID hostID, const Process &p)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO procs("
        "id_host, id_ref, path, name, md5, pid, uid, username, arguments"
        ") VALUES (";
  ss << hostID << ",";
  addReferenceURL(ss, p.getReferenceURL() );
  ss << ",";
  ap.append(ss, p.getPath().get() );
  ss << ",";
  ap.append(ss, p.getName().get() );
  ss << ",";
  ap.append(ss, (p.getMD5())?p.getMD5()->get():NULL);
  ss << ",";
  ap.append(ss, p.getPID() );
  ss << ",";
  ap.append(ss, p.getUID() );
  ss << ",";
  ap.append(ss, p.getUsername().get() );
  ss << ",";
  ap.append(ss, p.getParameters() );
  ss << ");";
  // insert object to data base.
  SQL( ss.str(), log_ ).exec(t_);

  return getID("procs_id_seq");
}

DataBaseID EntrySaver::getID(const std::string &seqName)
{
  assert( seqName==t_.getAPI<Postgres::TransactionAPI>().escape(seqName.c_str()) && "invalid sequence name" );

  const std::string sql="SELECT currval('" + seqName + "') as id;";
  const result r=SQL(sql, log_).exec(t_);
  assert( r.size()==1 && "unable to read current sequence number" );

  DataBaseID id;
  r[0]["id"].to(id);
  return id;
}

DataBaseID EntrySaver::getSeverityID(const Alert &a)
{
  const int level=a.getSeverity().getLevel().toInt();
  assert(level >= 0);
  assert(level <= 4);
  return level;
}

void EntrySaver::addReferenceURL(std::stringstream &ss, ConstReferenceURLPtr url)
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
  Appender     ap(t_);
  ss << "SELECT * FROM analyzers WHERE name = ";
  ap.append(ss, a.getName().get() );
  ss << " AND version ";
  addEqualityComparison(ss, ap, a.getVersion() );
  ss << " AND os";
  addEqualityComparison(ss, ap, a.getOperatingSystem() );
  ss << " AND ip";
  addEqualityComparison(ss, ap, a.getIP() );
  ss << " AND sys_id=";
  ap.append(ss, a.getID().get() );
  ss << ";";
  const result r=SQL( ss.str(), log_ ).exec(t_);
  if( r.empty() )
  {
    LOGMSG_DEBUG_S(log_)<< "analyzer " << a.getID().get() << " ('"<<a.getName().get()
                        << "') not found in data base";
    return Base::NullValue<DataBaseID>();
  }
  r[0]["id"].to(id);
  LOGMSG_DEBUG_S(log_)<< "analyzer " << a.getID().get() << " ('" << a.getName().get()
                      << "') found in data base (db-ID is " << id << ")";
  return Base::NullValue<DataBaseID>( id );
}

DataBaseID EntrySaver::saveReferenceURL(const ReferenceURL &url)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO reference_urls(name, url) VALUES (";
  ap.append(ss, url.getName().get() );
  ss << ",";
  ap.append(ss, url.getURL().get() );
  ss << ");";
  // insert object to data base.
  SQL( ss.str(), log_ ).exec(t_);

  return getID("reference_urls_id_seq");
}

DataBaseID EntrySaver::saveHostGeneric(DataBaseID alertID, const Persistency::Host &h, const char *role)
{
  assert(role!=NULL);
  assert(strcmp(role, "src") == 0 || strcmp(role, "dst") == 0);

  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO hosts(id_alert, id_ref, role, ip, mask, os, name) VALUES (";
  ss << alertID << ",";
  addReferenceURL(ss, h.getReferenceURL() );
  ss << ",";
  ap.append(ss, role);
  ss << ",";
  ap.append(ss, h.getIP().to_string() );
  ss << ",";
  ap.append(ss, h.getNetmask()?h.getNetmask()->to_string().c_str():NULL );
  ss << ",";
  ap.append(ss, h.getOperatingSystem().get() );
  ss << ",";
  ap.append(ss, h.getName().get() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
  return getID("hosts_id_seq");
}

DataBaseID EntrySaver::saveTargetHost(DataBaseID alertID, const Persistency::Host &h)
{
  return saveHostGeneric(alertID, h, "dst");
}

DataBaseID EntrySaver::saveSourceHost(DataBaseID alertID, const Persistency::Host &h)
{
  return saveHostGeneric(alertID, h, "src");
}

DataBaseID EntrySaver::saveAlert(const Persistency::Alert &a)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO alerts(name, detect_time, create_time, severity, certainty, description) VALUES (";
  ap.append(ss, a.getName().get() );
  ss << ",";
  ap.append(ss, a.getDetectionTime()?a.getDetectionTime():NULL);
  ss << ",";
  ap.append(ss, a.getCreationTime() );
  ss << ",";
  const DataBaseID sevID = getSeverityID(a);
  ap.append(ss, sevID);
  ss << ",";
  ap.append(ss, a.getCertainty().get() );
  ss << ",";
  ap.append(ss, a.getDescription() );
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
  Appender     ap(t_);
  ss << "INSERT INTO analyzers(sys_id, name, version, os, ip) VALUES (";
  ap.append(ss, a.getID().get() );
  ss << ",";
  ap.append(ss, a.getName().get() );
  ss << ",";
  ap.append(ss, a.getVersion().get() );
  ss << ",";
  ap.append(ss, a.getOperatingSystem().get() );
  ss << ",";
  ap.append(ss, a.getIP()?( a.getIP()->to_string().c_str() ):NULL);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
  return getID("analyzers_id_seq");
}

DataBaseID EntrySaver::saveService(DataBaseID hostID, const Service &s)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO services(id_host, id_ref, name, port, protocol) VALUES (";
  ss << hostID << ",";
  addReferenceURL(ss, s.getReferenceURL() );
  ss << ",";
  ap.append(ss, s.getName().get() );
  ss << ",";
  ap.append(ss, s.getPort().get() );
  ss <<",";
  ap.append(ss, s.getProtocol().get() );
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);

  return getID("services_id_seq");
}

DataBaseID EntrySaver::saveMetaAlert(const Persistency::MetaAlert &ma)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO meta_alerts(sys_id, id_ref, name, severity_delta, certainty_delta, create_time, last_update_time) VALUES (";
  ap.append(ss, ma.getID().get() );
  ss << ",";
  addReferenceURL(ss, ma.getReferenceURL() );
  ss << ",";
  ap.append(ss, ma.getName().get() );
  ss << ",";
  ap.append(ss, ma.getSeverityDelta() );
  ss << ",";
  ap.append(ss, ma.getCertaintyDelta() );
  ss << ",";
  ap.append(ss, ma.getCreateTime() );
  ss << ",";
  ap.append(ss, "now()");
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);

  return getID("meta_alerts_id_seq");
}

void EntrySaver::saveAlertToMetaAlertMap(DataBaseID alertID, DataBaseID malertID)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO alert_to_meta_alert_map(id_alert, id_meta_alert) VALUES(";
  ap.append(ss, alertID);
  ss << ",";
  ap.append(ss, malertID);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::saveAlertToAnalyzers(DataBaseID alertID, DataBaseID anlzID)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO alert_analyzers(id_alert, id_analyzer) VALUES (";
  ap.append(ss, alertID);
  ss << ",";
  ap.append(ss, anlzID);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}


void EntrySaver::saveMetaAlertsTree(DataBaseID nodeID, DataBaseID childID)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO meta_alerts_tree(id_node, id_child) VALUES (";
  ap.append(ss, nodeID);
  ss << ",";
  ap.append(ss, childID);
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
  stringstream ss;
  ss << "DELETE FROM meta_alerts_in_use WHERE id_meta_alert = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::markMetaAlertAsTriggered(DataBaseID malertID, const std::string &name)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO meta_alerts_already_triggered(id_meta_alert, trigger_name) VALUES(";
  ap.append(ss, malertID);
  ss << ",";
  ap.append(ss, name);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::removeMetaAlertFromTriggered(DataBaseID malertID)
{
  stringstream ss;
  ss << "DELETE FROM  meta_alerts_already_triggered WHERE id_meta_alert = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::updateSeverityDelta(DataBaseID malertID, double severityDelta)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "UPDATE meta_alerts SET severity_delta = severity_delta + ";
  ap.append(ss, severityDelta);
  ss << ", last_update_time = now() ";
  ss << " WHERE id = " << malertID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::updateCertaintyDelta(DataBaseID malertID, double certaintyDelta)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "UPDATE meta_alerts SET certainty_delta = certainty_delta + ";
  ap.append(ss, certaintyDelta);
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
  Appender     ap(t_);
  ss << "UPDATE hosts SET name = ";
  ap.append(ss, name.get());
  ss << " WHERE id = " << hostID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}


// helpers call updating given config entry
namespace
{
int updateConfigValue(const DynamicConfig::Owner &owner,
                      const DynamicConfig::Key   &key,
                      const DynamicConfig::Value &value,
                      const Logger::Node         &log,
                      Transaction                &t)
{
  stringstream ss;
  Appender     ap(t);
  ss << "UPDATE config SET value = ";
  ap.append(ss, value.get());
  ss << " WHERE owner";
  addEqualityComparison( ss, ap, owner.get() );
  ss << " AND key = ";
  ap.append(ss, key.get());
  // update entry and return number of affected rows
  return SQL( ss.str().c_str(), log ).exec(t).affected_rows();
} // updateConfigValue()
} // unnamed namespace

void EntrySaver::saveConfigParameter(const DynamicConfig::Owner &owner,
                                     const DynamicConfig::Key   &key,
                                     const DynamicConfig::Value &value)
{
  // if it's not present, add new one
  if( updateConfigValue(owner, key, value, log_, t_)!=0 )
  {
    LOGMSG_DEBUG(log_, "update changed record in first run - entry has been updated");
    return;
  }
  // if the value was NOT updated, it appears that proper entry does not exist
  // in data base yet, thou it must be added, BUT here we have race condition
  // if multiple threads were addining the data. to avoid this explicit table
  // locking is required (note: lock will be autoamatically released uppon
  // transaction end (commit/rollback)).
  // 'SHARE UPDATE EXCLUSIVE' mode means that no one can do concurrent updates to
  // this table, but everyone can read from it
  // (see http://www.postgresql.org/docs/8.4/interactive/explicit-locking.html for details).
  LOGMSG_DEBUG(log_, "update didn't changed any records - locking 'config' table explicitly");
  SQL("LOCK TABLE config IN SHARE UPDATE EXCLUSIVE MODE", log_).exec(t_);
  // now, having the lock aquired, we can ensure, that value we try to add is
  // truely exclisive - run 'UPDATE' once more and check the output.
  if( updateConfigValue(owner, key, value, log_, t_)==1 )
  {
    LOGMSG_DEBUG(log_, "second call to update changed 1 entry - nothing more to be done");
    return;
  }
  // if second update didn't change any value, after the table has been locked,
  // it means that we're the very first thread to add this to the data base.
  LOGMSG_DEBUG(log_, "second call to update didn't changed any entry - new one must be added instead");
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO config (owner, key, value) VALUES (";
  ap.append(ss, owner.get());
  ss << ",";
  ap.append(ss, key.get());
  ss << ",";
  ap.append(ss, value.get());
  ss << ")";
  SQL( ss.str().c_str(), log_ ).exec(t_);
  LOGMSG_DEBUG(log_, "new entry has been added"); //TODO: Fix message. When transaction is rolled back nothing is added.
}

void EntrySaver::saveRootID(DataBaseID rootID)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO meta_alerts_roots(id_root) VALUES(";
  ap.append(ss, rootID);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::deleteRootID(DataBaseID rootID)
{
  stringstream ss;
  ss << "DELETE FROM meta_alerts_roots WHERE id_root = " << rootID << ";";
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::removeConfigParameter(const DynamicConfig::Owner &owner,
                                       const DynamicConfig::Key   &key)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "DELETE FROM config WHERE owner = ";
  ap.append(ss, owner.get());
  ss << " AND key = ";
  ap.append(ss, key.get());
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::deleteHeartbeat(const Persistency::IO::Heartbeats::Owner  &owner,
                                 const Persistency::IO::Heartbeats::Module &module)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "DELETE FROM heartbeats WHERE owner = ";
  ap.append(ss, owner.get() );
  ss << " AND module = ";
  ap.append(ss, module.get() );
  SQL( ss.str(), log_ ).exec(t_);
}

void EntrySaver::saveHeartbeat(const Persistency::IO::Heartbeats::Owner  &owner,
                               const Persistency::IO::Heartbeats::Module &module,
                               Persistency::Timestamp                     reported,
                               const unsigned int                         timeout)
{
  stringstream ss;
  Appender     ap(t_);
  ss << "INSERT INTO heartbeats(owner, module, timestamp, timeout) VALUES(";
  ap.append(ss, owner.get() );
  ss << ",";
  ap.append(ss, module.get() );
  ss << ",";
  ap.append(ss, reported);
  ss << ",";
  ap.append(ss, timeout);
  ss << ");";
  SQL( ss.str(), log_ ).exec(t_);
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency
