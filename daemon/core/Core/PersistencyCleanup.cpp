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

PersistencyCleanup::PersistencyCleanup(void):
  log_("core.persistencycleanup")
{
  LOGMSG_INFO(log_, "created");
  cleanup();
}

void PersistencyCleanup::cleanup(void)
{
  LOGMSG_WARN(log_, "calling cleanup procedure");
  ConnectionPtrNN conn   =static_cast<ConnectionPtrNN>( create() );
  const size_t    limit  =3*31;     // TODO: this should be read from config file
  Transaction     t( conn->createNewTransaction("cleanup_procedure") );
  const size_t    removed=conn->removeEntriesOlderThan(limit, t);
  t.commit();
  LOGMSG_WARN_S(log_)<<"cleanup done - "<<removed<<" alerts removed";
}

} // namespace Core
