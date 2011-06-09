/*
 * PersistencyCleanup.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/PersistencyCleanup.hpp"

using namespace Persistency::IO;

namespace Core
{

PersistencyCleanup::PersistencyCleanup(ConfigIO::GeneralConfig::Interval cleanupOlder):
  log_("core.persistencycleanup"),
  cleanupOlder_(cleanupOlder)
{
  LOGMSG_INFO_S(log_)<<"created with clenaup older than "<<cleanupOlder_<<" days"<<(cleanupOlder_==0?" (never)":"");
}

void PersistencyCleanup::cleanup(void)
{
  // check if we have to cleanup at all
  if(cleanupOlder_==0u)
  {
    LOGMSG_INFO(log_, "cleanup procedure disabled, thus not called");
    return;
  }
  // ok - cleanup!
  LOGMSG_WARN(log_, "calling cleanup procedure");
  ConnectionPtrNN conn   =static_cast<ConnectionPtrNN>( create() );
  Transaction     t( conn->createNewTransaction("cleanup_procedure") );
  const size_t    removed=conn->removeEntriesOlderThan(cleanupOlder_, t);
  t.commit();
  LOGMSG_INFO_S(log_)<<"cleanup done - "<<removed<<" alerts removed";
}

} // namespace Core
