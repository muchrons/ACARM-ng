/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Filter/DNSResolver/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/DNSResolver/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace DNSResolver
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Parameters &params):
  Filter::Strategy<Data>(Core::Types::Proc::TypeName("dnsresolver"), name),
  cache_(params.cacheTimeout_)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( Core::Types::Proc::TypeName(ConfigConsts::defaultInputTypeName) );
  return ecl;
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendFacade      &bf)
{
  Algo::forEachUniqueLeaf(n, EntryProcessor(&cache_, &bf) );
}

} // namespace DNSResolver
} // namespace Filter
