/*
 * TestDBAccress.t.cpp
 *
 */
#include <pqxx/pqxx>

#include "Persistency/IO/Postgres/TestDBAccess.t.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

TestDBAccess::TestDBAccess(void):
  conn_( DBConnection::Parameters("localhost",
                                  "acarm_ng_test",
                                  "acarm-ng-tests",
                                  "test.password") )
{
}

void TestDBAccess::removeAllData(void)
{
  pqxx::work t(conn_.get(), "remove_all_data");
  t.exec("DELETE FROM meta_alerts_in_use");
  t.exec("DELETE FROM alert_to_meta_alert_map");
  t.exec("DELETE FROM meta_alerts_tree");
  t.exec("DELETE FROM meta_alerts");
  t.exec("DELETE FROM reported_procs");
  t.exec("DELETE FROM reported_services");
  t.exec("DELETE FROM reported_hosts");
  t.exec("DELETE FROM alerts");
  t.exec("DELETE FROM severities");
  t.exec("DELETE FROM analyzers");
  t.exec("DELETE FROM hosts");
  t.exec("DELETE FROM reference_urls");
  t.exec("DELETE FROM services");
  t.exec("DELETE FROM procs");
  t.exec("DELETE FROM config");
  t.exec("DELETE FROM config_rdonly");
  t.exec("DELETE FROM logger_logs");
  t.exec("DELETE FROM logger_nodes");
  t.commit();
}

void TestDBAccess::fillWithContent1(void)
{
  pqxx::work t(conn_.get(), "fill_with_content1");
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
