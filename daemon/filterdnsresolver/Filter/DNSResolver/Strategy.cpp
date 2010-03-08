/*
 * Strategy.cpp
 *
 */
#include "Filter/DNSResolver/Strategy.hpp"
#include <cassert>
#include "Algo/forEachUniqueLeaf.hpp"
//#include "Filter/DNSResolver/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace DNSResolver
{

Strategy::Strategy(const Parameters &/*params*/):
  Filter::Strategy<Data>("dnsresolver")
{
}

void Strategy::processImpl(Node               /*n*/,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendProxy      &/*bp*/)
{
}

} // namespace DNSResolver
} // namespace Filter
