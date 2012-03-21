/*
 * PersistencyPeriodicQueries.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/PersistencyPeriodicQueries.hpp"

using namespace Persistency::IO;

namespace Core
{

PersistencyPeriodicQueries::PersistencyPeriodicQueries():
  log_("core.persistencyperiodicqueries")
{
  LOGMSG_INFO_S(log_)<<"created"; //with n user queries
}

void PersistencyPeriodicQueries::issue(void)
{
  LOGMSG_INFO(log_, "calling periodic queries");

  ConnectionPtrNN conn=static_cast<ConnectionPtrNN>(create());
  Transaction     t(conn->createNewTransaction("periodic_queries"));
  conn->issuePeriodicQueries(t);
  t.commit();

  LOGMSG_INFO(log_, "running of periodic queries has finished");
}

} // namespace Core
