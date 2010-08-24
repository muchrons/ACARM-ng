/*
 * BackendFacade.cpp
 *
 */
#include <cassert>

#include "Filter/BackendFacade.hpp"
#include "System/Threads/SafeInitLocking.hpp"
#include "Persistency/IDAssigner.hpp"

using namespace Persistency;


namespace Filter
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                           ChangedNodes                     &changed,
                           const std::string                &filterName):
  Core::Types::BackendFacade(conn, filterName),
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

  // source or destination host?
  if( hasHost( a->getReportedSourceHosts(), host) ||
      hasHost( a->getReportedTargetHosts(), host)    )
    return true;

  return false;
} // ensureHostIsFromNode()

} // unnamed namespace

void BackendFacade::setHostName(Node                    node,
                               Persistency::HostPtrNN  host,
                               const std::string      &name)
{
  assert( isHostFromNode(node, host) );
  beginTransaction();
  IO::HostAutoPtr io=getConnection()->host(host, getTransaction() );
  io->setName(name);
  changed_.push_back(node);
}

void BackendFacade::updateSeverityDelta(Node         node,
                                       const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateSeverityDelta(delta);
  changed_.push_back(node);
}

void BackendFacade::updateCertaintyDelta(Node         node,
                                        const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateCertaintyDelta(delta);
  changed_.push_back(node);
}

void BackendFacade::addChild(Node parent, Node child)
{
  beginTransaction();
  MetaAlertPtrNN       ma=parent->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  parent->addChild(child, *io);
  changed_.push_back(parent);
}

Persistency::GraphNodePtrNN BackendFacade::correlate(
            Persistency::MetaAlertPtrNN  ma,
            const ChildrenVector        &children)
{
  beginTransaction();
  Node ptr( new GraphNode(ma, getConnection(), getTransaction(), children) );
  changed_.push_back(ptr);
  return ptr;
}

Persistency::MetaAlert::ID BackendFacade::getNextFreeID(void)
{
  // this is nasty global mutex, that ensures this method will be called by at most one
  // instance at a time this is important at this point, since non-atomic operations (add/del)
  // could lead to inconsistentcy in persistency storage (i.e. the same value could be assigned
  // to more than one meta-alert).
  SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(mutex);
  System::Threads::SafeInitLock lock(mutex);
  // here goes "real" code:
  beginTransaction();
  IDAssigner idas( getConnection(), getTransaction() );
  return idas.assign();
}

} // namespace Filter
