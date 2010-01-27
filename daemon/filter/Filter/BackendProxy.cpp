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
  Core::Types::Proc::BackendProxy(conn, filterName)
{
}

void BackendProxy::setHostName(Persistency::HostPtrNN host, const std::string &name)
{
  beginTransaction();
  IO::HostAutoPtr io=getConnection()->host(host, getTransaction() );
  io->setName(name);
}

void BackendProxy::updateSeverityDelta(Persistency::MetaAlertPtrNN ma, double delta)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateSeverityDelta(delta);
}

void BackendProxy::updateCertaintyDelta(Persistency::MetaAlertPtrNN ma, double delta)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateCertaintyDelta(delta);
}

void BackendProxy::addChild(Persistency::GraphNodePtrNN parent,
                            Persistency::GraphNodePtrNN child)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert( parent->getMetaAlert(), getTransaction() );
  parent->addChild(child, *io);
}

Persistency::GraphNodePtrNN BackendProxy::correlate(
            Persistency::MetaAlertPtrNN  ma,
            Persistency::GraphNodePtrNN  child1,
            Persistency::GraphNodePtrNN  child2,
            const ChildrenVector        &otherChildren)
{
  beginTransaction();
  GraphNodePtrNN ptr( new GraphNode(ma, getConnection(), getTransaction(),
                                    child1, child2, otherChildren) );
  return ptr;
}

} // namespace Filter
