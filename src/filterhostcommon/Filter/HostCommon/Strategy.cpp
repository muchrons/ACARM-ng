/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/HostCommon/Strategy.hpp"
#include "Filter/HostCommon/CheckHosts.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;

namespace Filter
{
namespace HostCommon
{

Strategy::Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name, unsigned int timeout):
  Filter::Simple::Strategy<Data>(type, name, timeout)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName(ConfigConsts::defaultInputTypeName) );
  return ecl;
}

Persistency::ConstHostPtr Strategy::getHost(const Node node) const
{
  return Algo::forEachUniqueLeaf( node, CheckHosts(this) ).out_;
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  // get host from this node.
  ConstHostPtr h=getHost(n);
  return Data(h);
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  // must be common host
  return thisEntry.t_.host_.get()!=NULL;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry /*otherEntry*/) const
{
  assert( thisEntry.t_.host_.get() !=NULL );
  // forward to user's implementation
  return getMetaAlertName(thisEntry.t_.host_);
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  assert( thisEntry.t_.host_.get() !=NULL );
  assert( otherEntry.t_.host_.get()!=NULL );
  // common host entries must equal
  return thisEntry.t_.host_->getIP() == otherEntry.t_.host_->getIP();
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &/*otherEntry*/,
                                      const Node       /*newNode*/) const
{
  return thisEntry.t_;
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace HostCommon
} // namespace Filter
