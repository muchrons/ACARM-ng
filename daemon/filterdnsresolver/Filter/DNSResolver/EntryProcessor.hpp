/*
 * EntryProcessor.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_ENTRYPROCESSOR_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_ENTRYPROCESSOR_HPP_FILE

#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/DNSResolver/CachedDNS.hpp"

namespace Filter
{
namespace DNSResolver
{

/** \brief helper object for processing given leaf.
 *
 *  processing in this context means reverse mapping IPs to DNS names
 *  for each host (source or destination) attached to given leaf.
 */
class EntryProcessor
{
public:
  /** \brief create instance.
   *  \param cache cache to be used for mappings.
   *  \param bf    facade for writing new names of hosts to persistency.
   */
  EntryProcessor(CachedDNS *cache, BackendFacade *bf);
  /** \brief method responsible for doing all the job.
   *  \param leaf leaft to be processed.
   */
  void operator()(Persistency::GraphNodePtrNN leaf);

private:
  void processHosts(Persistency::GraphNodePtrNN              leaf,
                    const Persistency::Alert::ReportedHosts &rh);

  CachedDNS     *cache_;
  BackendFacade *bf_;
}; // class EntryProcessor

} // namespace DNSResolver
} // namespace Filter

#endif
