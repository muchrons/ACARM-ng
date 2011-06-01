/*
 * BackendFacade.cpp
 *
 */
#include <algorithm>
#include <cassert>

#include "Filter/BackendFacade.hpp"
#include "Persistency/Facades/IDAssigner.hpp"

using namespace Persistency;


namespace Filter
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN     conn,
                           ChangedNodes                          &changed,
                           const Core::Types::Proc::TypeName     &filterType,
                           const Core::Types::Proc::InstanceName &filterName):
  Core::Types::BackendFacade(conn, filterType, filterName),
  changed_(changed)
{
  if( changed_.size()!=0 )
    throw ExceptionChangedNodesNotEmpty(SYSTEM_SAVE_LOCATION, filterName.str().c_str() );
}

namespace
{

bool hasHost(const Persistency::Alert::Hosts &rh, HostPtrNN ptr)
{
  for(Persistency::Alert::Hosts::const_iterator it=rh.begin();
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
  if( hasHost( a->getSourceHosts(), host) ||
      hasHost( a->getTargetHosts(), host)    )
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
  markNodeAsChanged(node);
}

void BackendFacade::updateSeverityDelta(Node         node,
                                        const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateSeverityDelta(delta);
  markNodeAsChanged(node);
}

void BackendFacade::updateCertaintyDelta(Node         node,
                                         const double delta)
{
  beginTransaction();
  MetaAlertPtrNN       ma=node->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->updateCertaintyDelta(delta);
  markNodeAsChanged(node);
}

void BackendFacade::addChild(Node parent, Node child)
{
  beginTransaction();
  MetaAlertPtrNN       ma=parent->getMetaAlert();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  parent->addChild(child, *io);
  markNodeAsChanged(parent);
}

Persistency::GraphNodePtrNN BackendFacade::correlate(
            Persistency::MetaAlertPtrNN  ma,
            const ChildrenVector        &children)
{
  beginTransaction();
  Node ptr( new GraphNode(ma, getConnection(), getTransaction(), children) );
  markNodeAsChanged(ptr);
  return ptr;
}

Persistency::MetaAlert::ID BackendFacade::getNextFreeID(void)
{
  beginTransaction();
  return Facades::IDAssigner::get()->assignMetaAlertID( getConnection(), getTransaction() );
}

void BackendFacade::markNodeAsChanged(Node node)
{
  // if node has not been reported, mark it as changed
  if( std::find( changed_.begin(), changed_.end(), node )==changed_.end() )
    changed_.push_back(node);
}

Persistency::IO::DynamicConfigAutoPtr BackendFacade::createDynamicConfig(const Persistency::IO::DynamicConfig::Owner &owner)
{
  return Core::Types::BackendFacade::createDynamicConfig( owner );
}

} // namespace Filter
