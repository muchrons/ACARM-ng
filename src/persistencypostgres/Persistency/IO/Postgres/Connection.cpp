/*
 * Connection.cpp
 *
 */
#include <sstream>

#include "System/TimerThreadCPU.hpp"
#include "System/TimerRT.hpp"
#include "System/ignore.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

using namespace std;


namespace Persistency
{
namespace IO
{
namespace Postgres
{

// helper calls
namespace
{

inline pqxx::result execSQL(const Logger::Node &log, Transaction &t, const char *sql)
{
  LOGMSG_DEBUG_S(log)<<"calling SQL statement: "<<sql;
  System::ignore(log);      // this suppresses warning in release mode about unsued parameter
  const pqxx::result r=t.getAPI<TransactionAPI>().exec(sql);
  LOGMSG_DEBUG_S(log)<<"affected rows: "<<r.affected_rows();
  return r;
} // execSQL()


void createTempTable(Transaction &t, const Logger::Node &log, const std::string &name, const char *sql=NULL)
{
  // create temporary table
  {
    stringstream ss;
    ss << "CREATE TEMP TABLE " << name;
    if(sql==NULL)
      ss << "(id int NOT NULL)";
    ss << " ON COMMIT DROP";
    if(sql!=NULL)
      ss << " AS (" << sql << ")";
    execSQL(log, t, ss.str().c_str() );
  }

  // create index for this table
  {
    stringstream ss;
    ss << "CREATE INDEX " << name << "_id_index ON " << name << "(id)";
    execSQL(log, t, ss.str().c_str() );
  }
}


void createTemporaryTables(size_t days, Transaction &t, const Logger::Node &log)
{
  // build and execute first statement, that creates sql query for gathering
  // IDs to be removed.
  {
    stringstream ss;
    ss << "SELECT id FROM alerts WHERE create_time < now() - interval '"
       << days << " day' AND id NOT IN"
          "      ( SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert IN"
          "        ( SELECT id_meta_alert FROM meta_alerts_in_use )"
          "      )";
    createTempTable(t, log, "cleanup_alerts_ids", ss.str().c_str() );
  }

  // reported hosts' IDs
  createTempTable(t, log, "cleanup_hosts_ids",
                  "SELECT id FROM hosts WHERE id_alert IN (SELECT id FROM cleanup_alerts_ids)");

  // meta alerts' IDs
  createTempTable(t, log, "cleanup_meta_alerts_ids",
                  "SELECT id_meta_alert AS id FROM alert_to_meta_alert_map WHERE id_alert IN (SELECT id FROM cleanup_alerts_ids)");

  // TODO: add debug check if cleanup_alerts_ids and cleanup_meta_alerts_ids have equal count
}


void removeServices(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM services WHERE id_host IN (SELECT id FROM cleanup_hosts_ids)");
}


void removeProcs(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM procs WHERE id_host IN (SELECT id FROM cleanup_hosts_ids)");
}


void removeHosts(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM hosts WHERE id IN (SELECT id FROM cleanup_hosts_ids)");
}


void removeAnalyzers(Transaction &t, const Logger::Node &log)
{
  // analyzers to check for removal
  createTempTable(t, log, "cleanup_candidates_to_remove_analyzers_ids",
                  "SELECT id_analyzer AS id FROM alert_analyzers WHERE id_alert IN (SELECT id FROM cleanup_alerts_ids)");
  // remove mappings
  execSQL(log, t, "DELETE FROM alert_analyzers WHERE id_alert IN (SELECT id FROM cleanup_alerts_ids)");
  // removes analyzers, but only those that belonged to removed alerts and are not used by others any more
  execSQL(log, t, "DELETE FROM analyzers WHERE"
                  "  id IN (SELECT id FROM cleanup_candidates_to_remove_analyzers_ids WHERE"
                  "           id NOT IN (SELECT id_analyzer FROM alert_analyzers))");
}


size_t removeAlerts(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM alert_to_meta_alert_map WHERE id_alert "
                  " IN (SELECT id FROM cleanup_alerts_ids)");
  // finaly remove all alerts, that are not used
  const size_t removed=execSQL(log, t, "DELETE FROM alerts WHERE id "
                                       " IN (SELECT id FROM cleanup_alerts_ids)").affected_rows();
  return removed;
}


void removeExtraMetaAlertsEntries(Transaction &t, const Logger::Node &log)
{
  // table holding IDs that are to be checked, ifthey can be removed or not
  createTempTable(t, log, "cleanup_candidates_to_remove_ma_ids");
  // table containing IDs that are known to be removed from previous iterations. initially
  // filled up with leafs to be deleted.
  createTempTable(t, log, "cleanup_new_to_remove_ma_ids", "SELECT * FROM cleanup_meta_alerts_ids");

  // loot pruning unneeded elements from tree up to the roots
  size_t affected;
  do
  {
    // clean-up candidates list before proceeding
    execSQL(log, t, "DELETE FROM cleanup_candidates_to_remove_ma_ids");
    // save all parents of childrent to be removed - if they stay empty, they are to be removed as well!
    execSQL(log, t, "INSERT INTO cleanup_candidates_to_remove_ma_ids"
                    "  SELECT id_node AS id FROM meta_alerts_tree WHERE id_child IN"
                    "    (SELECT id FROM cleanup_new_to_remove_ma_ids)");

    // remove all tree entries, related to meta-alerts to be removed
    execSQL(log, t, "DELETE FROM meta_alerts_tree WHERE id_child IN (SELECT id FROM cleanup_new_to_remove_ma_ids)");
    execSQL(log, t, "DELETE FROM meta_alerts_tree WHERE id_node  IN (SELECT id FROM cleanup_new_to_remove_ma_ids)");

    // preapre temporary helper tables to accept new data
    execSQL(log, t, "DELETE FROM cleanup_new_to_remove_ma_ids");
    // check which parents are no longer in use, thus should be marked for removal
    affected =execSQL(log, t, "INSERT INTO cleanup_new_to_remove_ma_ids"
                              "  SELECT c.id FROM cleanup_candidates_to_remove_ma_ids AS c"
                              "    LEFT JOIN meta_alerts_tree AS t ON c.id=t.id_node").affected_rows();
    LOGMSG_DEBUG_S(log)<<"selected "<<affected<<" rows from candidates set";

    // remove roots that have only one child - they are invalid
    affected+=execSQL(log, t, "INSERT INTO cleanup_new_to_remove_ma_ids"
                              "  SELECT id_node AS id FROM"
                              "    (SELECT id_node, count(id_child) AS cnt FROM meta_alerts_tree GROUP BY id_node) AS s WHERE s.cnt=1").affected_rows();
    LOGMSG_DEBUG_S(log)<<"selected "<<affected<<" rows in total (i.e. parent nodes having only one child left)";

    // after the first run is done copy new IDs to be removed to main table
    execSQL(log, t, "INSERT INTO cleanup_meta_alerts_ids SELECT id FROM cleanup_new_to_remove_ma_ids");
    // add extra debugging info
    LOGMSG_DEBUG_S(log)<<"affected "<<affected<<" entries in total - "<<((affected>0)?"continuing interation":"loop's done");
  }
  while(affected>0);

  // remove meta-alerts already triggered
  execSQL(log, t, "DELETE FROM meta_alerts_already_triggered WHERE id_meta_alert IN"
                  " (SELECT id FROM cleanup_meta_alerts_ids)");
  // when no more IDs are to be removed from tree, remove meta-alerts them selfes
  execSQL(log, t, "DELETE FROM meta_alerts WHERE id IN (SELECT id FROM cleanup_meta_alerts_ids)");
}

} // unnamed namespace



Connection::Connection(DBHandlePtrNN handle):
  detail::ConnectionBase(handle),
  log_("persistency.io.postgres.connection")
{
  // in order for the string escaping mechanism to work as it should, strings must be set to
  // work in a SQL's standard-conformant way. otherwise bad things may happen.
  // this had to be added, since PostgreSQL 9.1 changed the default behaviour and, by default,
  // turned away from non-standard strings. and since escaping functions does not check this
  // setting, it has to be enforced manually...
  handle->getConnection().get().set_variable("standard_conforming_strings", "on");
  // in order to keep consistent time settings, when writing from daemon and reading from WUI,
  // common time zone (i.e. UTC) is enforced. as for the moment of writing these words, there
  // was no need for setting this explicitly, but it is safer, in case of future changes.
  handle->getConnection().get().set_variable("timezone", "UTC");
}

size_t Connection::removeEntriesOlderThanImpl(size_t days, Transaction &t)
{
  TRYCATCH_BEGIN
    System::TimerThreadCPU tcpu;
    System::TimerRT        trt;
    createTemporaryTables(days, t, log_);
    removeServices(t, log_);
    removeProcs(t, log_);
    removeHosts(t, log_);
    removeAnalyzers(t, log_);
    const size_t removed=removeAlerts(t, log_);
    removeExtraMetaAlertsEntries(t, log_);
    LOGMSG_INFO_S(log_)<<"data base cleanup took total time of "<<trt.elapsed()
                       <<"[s] (thread-CPU "<<tcpu.elapsed()<<"[s]); removed total of "<<removed<<" entries";
    return removed;
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
