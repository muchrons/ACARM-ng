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
  t.exec("DELETE FROM meta_alerts_already_triggered");
  t.exec("DELETE FROM meta_alerts_in_use");
  t.exec("DELETE FROM alert_to_meta_alert_map");
  t.exec("DELETE FROM meta_alerts_tree");
  t.exec("DELETE FROM meta_alerts");
  t.exec("DELETE FROM reported_procs");
  t.exec("DELETE FROM reported_services");
  t.exec("DELETE FROM reported_hosts");
  t.exec("DELETE FROM alert_analyzers");
  t.exec("DELETE FROM alerts");
  t.exec("DELETE FROM analyzers");
  t.exec("DELETE FROM hosts");
  t.exec("DELETE FROM reference_urls");
  t.exec("DELETE FROM services");
  t.exec("DELETE FROM procs");
  t.exec("DELETE FROM config");
  t.exec("DELETE FROM config_rdonly");
  t.exec("DELETE FROM logs");
  t.exec("DELETE FROM wui_users");
  t.commit();
}

void TestDBAccess::fillWithContent1(void)
{
  pqxx::work t(conn_.get(), "fill_with_content1");
  const int  ret=system("psql -d acarm_ng_test -f testdata/test_data_001.sql "
                        "> /dev/null");
  assert(ret==0 && "ooops - filling data base with tst content failed");
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
