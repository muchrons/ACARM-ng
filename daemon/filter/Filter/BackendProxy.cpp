/*
 * BackendProxy.cpp
 *
 */
#include <cassert>

#include "Filter/BackendProxy.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;


namespace Filter
{

BackendProxy::BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
                           const std::string                &filterName):
  filterName_(filterName),
  conn_(conn)
{
  // transaction is not started here yet - it will be initialized when needed
  // for the first time (not to do begin-rollback, useless traffic)
}

BackendProxy::~BackendProxy(void)
{
  // d-tor required to ensure proper destruction of forward-declared objects
}

void BackendProxy::setHostName(Persistency::HostPtrNN host, const std::string &name)
{
  beginTransaction();
  IO::HostAutoPtr io=conn_->host(host, getTransaction() );
  io->setName(name);
}

void BackendProxy::updateSeverityDelta(Persistency::MetaAlertPtrNN ma, double delta)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=conn_->metaAlert(ma, getTransaction() );
  io->updateSeverityDelta(delta);
}

void BackendProxy::updateCertanityDelta(Persistency::MetaAlertPtrNN ma, double delta)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=conn_->metaAlert(ma, getTransaction() );
  io->updateCertanityDelta(delta);
}

void BackendProxy::addChild(Persistency::GraphNodePtrNN parent,
                            Persistency::GraphNodePtrNN child)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=conn_->metaAlert( parent->getMetaAlert(), getTransaction() );
  parent->addChild(child, *io);
}

Persistency::GraphNodePtrNN BackendProxy::correlate(
            Persistency::MetaAlertPtrNN  ma,
            Persistency::GraphNodePtrNN  child1,
            Persistency::GraphNodePtrNN  child2,
            const ChildrenVector        &otherChildren)
{
  beginTransaction();
  GraphNodePtrNN ptr( new GraphNode(ma, conn_, getTransaction(),
                                    child1, child2, otherChildren) );
  return ptr;
}

void BackendProxy::commitChanges(void)
{
  // if no changes were introduced, just do nothing
  if( transaction_.get()==NULL )
    return;

  transaction_->commit();
}

void BackendProxy::beginTransaction(void)
{
  if( transaction_.get()==NULL )    // new transaction
  {
    TransactionAPIAutoPtr api=conn_->createNewTransaction(
                                "transaction_for_filter_" + filterName_);
    transaction_.reset( new Transaction(api) );
  }
  // if begin has been requested, transaction must always be valid
  transaction_->ensureIsActive();
}

Transaction &BackendProxy::getTransaction(void) const
{
  assert( transaction_.get()!=NULL );
  return *transaction_;
}

} // namespace Filter
