/*
 * ConnectionUserStubBase.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"

using namespace ::Persistency;
using namespace ::Persistency::IO;

namespace TestHelpers
{
namespace Persistency
{

ConnectionUserStubBase::ConnectionUserStubBase(void):
  genericStub_( BackendFactory::create( BackendFactory::FactoryTypeName("stubs"),
                                        BackendFactory::Options()
                                      ).release() )
{
}

::Persistency::IO::ConnectionPtrNN ConnectionUserStubBase::getStub(void)
{
  assert( genericStub_.get()!=NULL );
  return genericStub_;
}

TransactionAPIAutoPtr ConnectionUserStubBase::createNewTransactionImpl(Base::Threads::Mutex &/*mutex*/,
                                                                       const std::string    &name)
{
  return getStub()->createNewTransaction(name);
}

AlertAutoPtr ConnectionUserStubBase::alertImpl(AlertPtrNN alert, Transaction &t)
{
  return getStub()->alert(alert, t);
}

HostAutoPtr ConnectionUserStubBase::hostImpl(HostPtrNN host, Transaction &t)
{
  return getStub()->host(host, t);
}

MetaAlertAutoPtr ConnectionUserStubBase::metaAlertImpl(MetaAlertPtrNN ma, Transaction &t)
{
  return getStub()->metaAlert(ma, t);
}

DynamicConfigAutoPtr ConnectionUserStubBase::dynamicConfigImpl(const DynamicConfig::Owner &owner,
                                                               Transaction                &t)
{
  return getStub()->dynamicConfig(owner, t);
}

RestorerAutoPtr ConnectionUserStubBase::restorerImpl(Transaction &t)
{
  return getStub()->restorer(t);
}

size_t ConnectionUserStubBase::removeEntriesOlderThanImpl(size_t days, Transaction &t)
{
  return getStub()->removeEntriesOlderThan(days,t );
}

void ConnectionUserStubBase::issuePeriodicSystemQueriesImpl(Transaction &/*t*/)
{
  //TODO
}

void ConnectionUserStubBase::issuePeriodicUserQueriesImpl(Transaction &/*t*/)
{
  //TODO
}

HeartbeatsAutoPtr ConnectionUserStubBase::heartbeatsImpl(const Heartbeats::Owner &owner, Transaction &t)
{
  return getStub()->heartbeats(owner, t);
}

} // namespace Persistency
} // namespace TestHelpers
