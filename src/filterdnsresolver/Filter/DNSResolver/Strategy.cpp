/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Filter/DNSResolver/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/DNSResolver/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace DNSResolver
{

Strategy::Strategy(const std::string &name, const Parameters &params):
  Filter::Strategy<Data>("dnsresolver", name),
  cache_(params.cacheTimeout_)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
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
