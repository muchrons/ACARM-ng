/*
 * GlobalConnection.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/IO/GlobalConnection.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Base::Threads;

namespace Persistency
{
namespace IO
{

void GlobalConnectionImpl::markAsTriggered(MetaAlertPtrNN ma, const std::string &name)
{
  Lock lock(mutex_);
  Transaction t( conn_->createNewTransaction("global_connection_impl_markastriggered") );
  MetaAlertAutoPtr maio=conn_->metaAlert(ma, t);
  assert( maio.get()!=NULL );
  maio->markAsTriggered(name);
  t.commit();
}

void GlobalConnectionImpl::markAsUnused(MetaAlertPtrNN ma)
{
  Lock lock(mutex_);
  Transaction t( conn_->createNewTransaction("global_connection_impl_markasunused") );
  MetaAlertAutoPtr maio=conn_->metaAlert(ma, t);
  assert( maio.get()!=NULL );
  maio->markAsUnused();
  t.commit();
}

GlobalConnectionImpl::GlobalConnectionImpl(void):
  conn_( create() )
{
  assert( conn_.get()!=NULL );
}

} // namespace IO
} // namespace Persistency
