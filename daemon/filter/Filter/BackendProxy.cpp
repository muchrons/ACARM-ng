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
                           ChangedNodes                     &changed,
                           const std::string                &filterName):
  Core::Types::Proc::BackendProxy(conn, filterName),
  changed_(changed)
{
  if( changed_.size()!=0 )
    throw ExceptionChangedNodesNotEmpty(SYSTEM_SAVE_LOCATION,
                                        filterName.c_str() );
}

namespace
{

bool hasHost(const Persistency::Alert::ReportedHosts &rh, HostPtrNN ptr)
{
  for(Persistency::Alert::ReportedHosts::const_iterator it=rh.begin();
      it!=rh.end(); ++it)
    if( it->get()==ptr.get() )
      return true;
  return false;
} // hasHost()

bool isHostFromNode(GraphNodePtrNN node, HostPtrNN host)
{
  if( !node->isLeaf() )
    return false;

  // process alert
  AlertPtrNN a=node->getAlert();

  // analyzer's host?
  if( a->getAnalyzer().getHost().get()==host.get() )
    return true;

  // source or destination host?
  if( hasHost( a->getReportedSourceHosts(), host) ||
      hasHost( a->getReportedTargetHosts(), host)    )
    return true;

  return false;
} // ensureHostIsFromNode()

} // unnamed namespace

void BackendProxy::setHostName(Node                    node,
                               Persistency::HostPtrNN  host,
                               const std::string      &name)
{
  assert( isHostFromNode(node, host) );
  beginTransaction();
  IO::HostAutoPtr io=getConnection()->host(host, getTransaction() );
  io->setName(name);
  changed_.push_back(node);
}

void BackendProxy::updateSeverityDelta(Node         node,
                                       const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateSeverityDelta(delta);
  changed_.push_back(node);
}

void BackendProxy::updateCertaintyDelta(Node         node,
                                        const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateCertaintyDelta(delta);
  changed_.push_back(node);
}

void BackendProxy::addChild(Node parent, Node child)
{
  beginTransaction();
  MetaAlertPtrNN       ma=parent->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  parent->addChild(child, *io);
  changed_.push_back(parent);
}

Persistency::GraphNodePtrNN BackendProxy::correlate(
            Persistency::MetaAlertPtrNN  ma,
            Node                         child1,
            Node                         child2,
            const ChildrenVector        &otherChildren)
{
  beginTransaction();
  Node ptr( new GraphNode(ma,
                          getConnection(),
                          getTransaction(),
                          child1,
                          child2,
                          otherChildren) );
  changed_.push_back(ptr);
  return ptr;
}

} // namespace Filter
