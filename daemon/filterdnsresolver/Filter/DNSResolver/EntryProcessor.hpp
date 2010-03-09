/*
 * EntryProcessor.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_ENTRYPROCESSOR_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_ENTRYPROCESSOR_HPP_FILE

#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Filter/BackendProxy.hpp"
#include "Filter/DNSResolver/CachedDNS.hpp"

namespace Filter
{
namespace DNSResolver
{

class EntryProcessor
{
public:
  EntryProcessor(CachedDNS *cache, BackendProxy *bp);

  void operator()(Persistency::GraphNodePtrNN leaf);

private:
  void processHosts(Persistency::GraphNodePtrNN              leaf,
                    const Persistency::Alert::ReportedHosts &rh);

  CachedDNS    *cache_;
  BackendProxy *bp_;
}; // class EntryProcessor

} // namespace DNSResolver
} // namespace Filter

#endif
