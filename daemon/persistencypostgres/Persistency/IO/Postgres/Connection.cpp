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

Connection::Connection(DBHandlerPtrNN handler):
  detail::ConnectionBase(handler),
  log_("persistency.io.postgres.connection")
{
}

namespace
{
inline pqxx::result execSQL(const Logger::Node &log, Transaction &t, const char *sql)
{
  LOGMSG_DEBUG_S(log)<<"calling SQL statement: "<<sql;
  return t.getAPI<TransactionAPI>().exec(sql);
} // execSQL()
} // unnamed namespace

size_t Connection::removeEntriesOlderThanImpl(size_t days, Transaction &t)
{
  TRYCATCH_BEGIN
    createTemporaryTables(days, t);
    removeReportedServices(t);
    removeReportedProcs(t);
    removeReportedHosts(t);
    removeAnalyzers(t);
    const size_t removed=removeAlerts(t);
    removeExtraMetaAlertsEntries(t);
    return removed;
  TRYCATCH_END
}


void Connection::createTemporaryTables(size_t days, Transaction &t) const
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
    execSQL(log_, t, ss.str().c_str() );
  }

  // reported hosts' IDs
  execSQL(log_, t, "CREATE TEMP TABLE tmp_rh"
             " ON COMMIT DROP"
             " AS"
             " ( SELECT id FROM reported_hosts WHERE id_alert IN (SELECT id FROM tmp) )");

  // meta alerts' IDs
  execSQL(log_, t, "CREATE TEMP TABLE tmp_ma"
             " ON COMMIT DROP"
             " AS"
             " ( SELECT id_meta_alert FROM alert_to_meta_alert_map WHERE id_alert IN (SELECT id FROM tmp) )");
}


void Connection::removeExtraMetaAlertsEntries(Transaction &t) const
{
  // remove elements in tree until there are some more to be removed.
  execSQL(log_, t, "CREATE TEMP TABLE tmp_ma_swap (id int NOT NULL) ON COMMIT DROP");
  size_t affected;
  do
  {
    // save set of IDs that are to be (potentially) removed in next iteration.
    execSQL(log_, t, "INSERT INTO tmp_ma_swap "
               "  SELECT DISTINCT(id_node) as id FROM meta_alerts_tree WHERE id_child "
               "    IN (SELECT id_meta_alert FROM tmp_ma)");

    // delete leafs that are known to be removed.
    execSQL(log_, t, "DELETE FROM meta_alerts_tree WHERE id_child "
               " IN (SELECT id_meta_alert FROM tmp_ma)");
    // remove from list of candidates to be removed ones that
    // still have some children
    execSQL(log_, t, "DELETE FROM tmp_ma_swap WHERE id IN "
               " (SELECT id_node FROM meta_alerts_tree)");

    // remove meta alerts that were removed (as children) from tree
    execSQL(log_, t, "DELETE FROM meta_alerts WHERE id "
               " IN (SELECT id_meta_alert FROM tmp_ma)");

    // remove meta alerts that are not referenced from tree any more
    execSQL(log_, t, "DELETE FROM meta_alerts WHERE id "
               " NOT IN (SELECT id_node  FROM meta_alerts_tree) "
               " AND id "
               " NOT IN (SELECT id_child FROM meta_alerts_tree)"
               " AND id "
               " NOT IN (SELECT id_meta_alert FROM alert_to_meta_alert_map)");

    // move tmp_ma_swap's content to tmp_ma
    execSQL(log_, t, "DELETE FROM tmp_ma");
    affected=execSQL(log_, t, "INSERT INTO tmp_ma SELECT id FROM tmp_ma_swap"
                    ).affected_rows();
    execSQL(log_, t, "DELETE FROM tmp_ma_swap");
  }
  while(affected>0);
}

void Connection::removeReportedServices(Transaction &t) const
{
  execSQL(log_, t, "DELETE FROM reported_services WHERE id_reported_host "
             " IN (SELECT id FROM tmp_rh)");
  execSQL(log_, t, "DELETE FROM services          WHERE id "
             " NOT IN (SELECT id_service FROM reported_services)");
}

void Connection::removeReportedProcs(Transaction &t) const
{
  execSQL(log_, t, "DELETE FROM reported_procs WHERE id_reported_host "
             " IN (SELECT id FROM tmp_rh)");
  execSQL(log_, t, "DELETE FROM procs          WHERE id "
             " NOT IN (SELECT id_proc FROM reported_procs)");
}

void Connection::removeReportedHosts(Transaction &t) const
{
  execSQL(log_, t, "DELETE FROM reported_hosts WHERE id "
             " IN (SELECT id FROM tmp_rh)");
  execSQL(log_, t, "DELETE FROM hosts          WHERE id "
             " NOT IN (SELECT id_host FROM reported_hosts)");
}

void Connection::removeAnalyzers(Transaction &t) const
{
  execSQL(log_, t, "DELETE FROM alert_analyzers WHERE id_alert "
             " IN (SELECT id FROM tmp)");
  execSQL(log_, t, "DELETE FROM analyzers       WHERE id "
             " NOT IN (SELECT id_analyzer FROM alert_analyzers)");
}

size_t Connection::removeAlerts(Transaction &t) const
{
  execSQL(log_, t, "DELETE FROM alert_to_meta_alert_map WHERE id_alert "
             " IN (SELECT id FROM tmp)");
  // finaly remove all alerts, that are not used
  const size_t removed=execSQL(log_, t, "DELETE FROM alerts WHERE id "
                                  " IN (SELECT id FROM tmp)").affected_rows();
  return removed;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
