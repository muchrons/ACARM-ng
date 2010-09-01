/*
 * Connection.cpp
 *
 */
#include <sstream>

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

// passing anything to this call ignores argument
template<typename T>
inline void ignore(const T &)
{
} // ignore()

inline pqxx::result execSQL(const Logger::Node &log, Transaction &t, const char *sql)
{
  LOGMSG_DEBUG_S(log)<<"calling SQL statement: "<<sql;
  ignore(log);      // this suppresses warning in release mode about unsued parameter
  return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()


void createTemporaryTables(size_t days, Transaction &t, const Logger::Node &log)
{
  // build and execute first statement, that creates sql query for gathering
  // IDs to be removed.
  {
    stringstream ss;
    ss << "CREATE TEMP TABLE tmp"
          "  ON COMMIT DROP"
          "  AS"
          "    (SELECT id FROM alerts WHERE create_time < now() - interval '"
       << days << " day' AND id NOT IN"
          "      ( SELECT id_alert FROM alert_to_meta_alert_map WHERE id_meta_alert IN"
          "        ( SELECT id_meta_alert FROM meta_alerts_in_use )"
          "      )"
          "    );";
    execSQL(log, t, ss.str().c_str() );
  }

  // reported hosts' IDs
  execSQL(log, t, "CREATE TEMP TABLE tmp_rh"
                  " ON COMMIT DROP"
                  " AS"
                  " ( SELECT id FROM reported_hosts WHERE id_alert IN (SELECT id FROM tmp) )");

  // meta alerts' IDs
  execSQL(log, t, "CREATE TEMP TABLE tmp_ma"
                  " ON COMMIT DROP"
                  " AS"
                  " ( SELECT id_meta_alert FROM alert_to_meta_alert_map WHERE id_alert IN (SELECT id FROM tmp) )");
}


void removeExtraMetaAlertsEntries(Transaction &t, const Logger::Node &log)
{
  // remove elements in tree until there are some more to be removed.
  execSQL(log, t, "CREATE TEMP TABLE tmp_ma_swap (id int NOT NULL) ON COMMIT DROP");
  size_t affected;
  do
  {
    // save set of IDs that are to be (potentially) removed in next iteration.
    execSQL(log, t, "INSERT INTO tmp_ma_swap "
                    "  SELECT DISTINCT(id_node) as id FROM meta_alerts_tree WHERE id_child "
                    "    IN (SELECT id_meta_alert FROM tmp_ma)");

    // delete leafs that are known to be removed.
    execSQL(log, t, "DELETE FROM meta_alerts_tree WHERE id_child "
                    " IN (SELECT id_meta_alert FROM tmp_ma)");
    // remove from list of candidates to be removed ones that
    // still have some children
    execSQL(log, t, "DELETE FROM tmp_ma_swap WHERE id IN "
                    " (SELECT id_node FROM meta_alerts_tree)");

    // remove meta alerts that were removed (as children) from tree
    execSQL(log, t, "DELETE FROM meta_alerts WHERE id "
                    " IN (SELECT id_meta_alert FROM tmp_ma)");

    // remove meta alerts that are not referenced from tree any more
    // TODO: this is the most evil query out of all here - it is the longest one and is
    //       unaccetably long one - the first one to be optimized.
    execSQL(log, t, "DELETE FROM meta_alerts WHERE id "
                    " NOT IN (SELECT id_node  FROM meta_alerts_tree ORDER BY id_node) "
                    " AND id "
                    " NOT IN (SELECT id_child FROM meta_alerts_tree ORDER BY id_child)"
                    " AND id "
                    " NOT IN (SELECT id_meta_alert FROM alert_to_meta_alert_map ORDER BY id_meta_alert)");

    // move tmp_ma_swap's content to tmp_ma
    execSQL(log, t, "DELETE FROM tmp_ma");
    affected=execSQL(log, t, "INSERT INTO tmp_ma SELECT id FROM tmp_ma_swap"
                    ).affected_rows();
    execSQL(log, t, "DELETE FROM tmp_ma_swap");
  }
  while(affected>0);
}

void removeReportedServices(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM reported_services WHERE id_reported_host "
                  " IN (SELECT id FROM tmp_rh)");
  execSQL(log, t, "DELETE FROM services          WHERE id "
                  " NOT IN (SELECT id_service FROM reported_services)");
}

void removeReportedProcs(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM reported_procs WHERE id_reported_host "
                  " IN (SELECT id FROM tmp_rh)");
  execSQL(log, t, "DELETE FROM procs          WHERE id "
                  " NOT IN (SELECT id_proc FROM reported_procs ORDER BY id_proc)");
}

void removeReportedHosts(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM reported_hosts WHERE id "
                  " IN (SELECT id FROM tmp_rh)");
  // TODO: following query takes extreamly long
  execSQL(log, t, "DELETE FROM hosts          WHERE id "
                  " NOT IN (SELECT id_host FROM reported_hosts ORDER BY id_host)");
}

void removeAnalyzers(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM alert_analyzers WHERE id_alert "
                  " IN (SELECT id FROM tmp)");
  // TODO: following query takes a lot of time
  execSQL(log, t, "DELETE FROM analyzers       WHERE id "
                  " NOT IN (SELECT id_analyzer FROM alert_analyzers ORDER BY id_analyzer)");
}

size_t removeAlerts(Transaction &t, const Logger::Node &log)
{
  execSQL(log, t, "DELETE FROM alert_to_meta_alert_map WHERE id_alert "
                  " IN (SELECT id FROM tmp)");
  // finaly remove all alerts, that are not used
  const size_t removed=execSQL(log, t, "DELETE FROM alerts WHERE id "
                                       " IN (SELECT id FROM tmp)").affected_rows();
  return removed;
}

} // unnamed namespace



Connection::Connection(DBHandlePtrNN handle):
  detail::ConnectionBase(handle),
  log_("persistency.io.postgres.connection")
{
}

size_t Connection::removeEntriesOlderThanImpl(size_t days, Transaction &t)
{
// TODO: THIS IS TEMPORARY WORKAROUND - THIS HAS TO BE RE-IMPLEMENTED!
return 0;           


  // TODO: this while statement can be refactored to use 'IN' instead of 'NOT IN' in statements,
  //       which is MUCH faster, plus adding extra indexes if needed.
  TRYCATCH_BEGIN
    createTemporaryTables(days, t, log_);
    removeReportedServices(t, log_);
    removeReportedProcs(t, log_);
    removeReportedHosts(t, log_);
    removeAnalyzers(t, log_);
    const size_t removed=removeAlerts(t, log_);
    removeExtraMetaAlertsEntries(t, log_);
    return removed;
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
