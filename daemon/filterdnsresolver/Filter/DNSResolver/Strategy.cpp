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

Strategy::Strategy(const Parameters &params):
  Filter::Strategy<Data>("dnsresolver"),
  cache_(params.cacheTimeout_)
{
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendFacade      &bf)
{
  Algo::forEachUniqueLeaf(n, EntryProcessor(&cache_, &bf) );
}

} // namespace DNSResolver
} // namespace Filter
