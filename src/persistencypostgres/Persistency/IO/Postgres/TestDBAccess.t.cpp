/*
 * TestDBAccress.t.cpp
 *
 */
#include <pqxx/pqxx>
#include <cstdlib>

#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

TestDBAccess::TestDBAccess(void):
  conn_( DBConnection::Parameters("localhost",
                                  "5432",
                                  "acarm_ng_test",
                                  "acarm-ng-tests",
                                  "test.password") )
{
}

void TestDBAccess::removeAllData(void)
{
  pqxx::work t(conn_.get(), "remove_all_data");
  /*
  // TRUNCATE is MUCH faster than DELETE, but pqxx displays some messages on the screen
  // when calling it, making tests unreadable... :(
  t.exec("TRUNCATE meta_alerts_already_triggered CASCADE");
  t.exec("TRUNCATE meta_alerts_in_use CASCADE");
  t.exec("TRUNCATE alert_to_meta_alert_map CASCADE");
  t.exec("TRUNCATE meta_alerts_tree CASCADE");
  t.exec("TRUNCATE meta_alerts CASCADE");
  t.exec("TRUNCATE alert_analyzers CASCADE");
  t.exec("TRUNCATE alerts CASCADE");
  t.exec("TRUNCATE analyzers CASCADE");
  t.exec("TRUNCATE hosts CASCADE");
  t.exec("TRUNCATE reference_urls CASCADE");
  t.exec("TRUNCATE services CASCADE");
  t.exec("TRUNCATE procs CASCADE");
  t.exec("TRUNCATE config CASCADE");
  t.exec("TRUNCATE config_rdonly CASCADE");
  t.exec("TRUNCATE logs CASCADE");
  t.exec("TRUNCATE wui_users CASCADE");
  */
  t.exec("DELETE FROM meta_alerts_already_triggered");
  t.exec("DELETE FROM meta_alerts_in_use");
  t.exec("DELETE FROM alert_to_meta_alert_map");
  t.exec("DELETE FROM meta_alerts_tree");
  t.exec("DELETE FROM meta_alerts_roots");
  t.exec("DELETE FROM meta_alerts");
  t.exec("DELETE FROM procs");
  t.exec("DELETE FROM services");
  t.exec("DELETE FROM hosts");
  t.exec("DELETE FROM alert_analyzers");
  t.exec("DELETE FROM analyzers");
  t.exec("DELETE FROM alerts");
  t.exec("DELETE FROM reference_urls");
  t.exec("DELETE FROM config");
  t.exec("DELETE FROM config_rdonly");
  t.exec("DELETE FROM pradolog");
  t.exec("DELETE FROM heartbeats");
  t.commit();
}

void TestDBAccess::fillWithContent1(void)
{
  const int  ret=system("psql \"host=localhost dbname=acarm_ng_test user=acarm-ng-tests password=test.password\" -f testdata/test_data_001.sql > /dev/null");
  assert(ret==0 && "ooops - filling data base with test content failed");
}

void TestDBAccess::execSQL(const char *sql)
{
  pqxx::work t(conn_.get(), "exec_sql");
  t.exec(sql);
  t.commit();
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
