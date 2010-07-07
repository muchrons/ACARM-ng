/*
 * EntryProcessor.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Filter/DNSResolver/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace DNSResolver
{

EntryProcessor::EntryProcessor(CachedDNS *cache, BackendFacade *bf):
  log_("filter.dnsresolver.entryprocessor"),
  cache_(cache),
  bf_(bf)
{
  assert(cache_!=NULL);   // this is only internal implementation
  assert(bf_   !=NULL);   // this is only internal implementation
}

void EntryProcessor::operator()(Persistency::GraphNodePtrNN leaf)
{
  assert( leaf->isLeaf() && "wrong graph-passing algorithm choosen" );
  processHosts(leaf, leaf->getAlert()->getReportedSourceHosts() );
  processHosts(leaf, leaf->getAlert()->getReportedTargetHosts() );
}

void EntryProcessor::processHosts(Persistency::GraphNodePtrNN              leaf,
                                  const Persistency::Alert::ReportedHosts &rh)
{
  typedef Persistency::Alert::ReportedHosts::const_iterator ConstIterator;
  for(ConstIterator it=rh.begin(); it!=rh.end(); ++it)
  {
    HostPtrNN host=*it;             // non-const pointer to set host name
    if( host->getName()!=NULL )     // if host already have DNS name skip it
      continue;

    try
    {
      // if no entry, try getting one
      const CachedDNS::Entry e=(*cache_)[ host->getIP() ];
      if(e.first)                                         // has name?
        bf_->setHostName(leaf, host, e.second.get() );    // set it!
    }
    catch(const ExceptionUnableToResolve &ex)
    {
      // log and ignore it - there are other IPs to be resolved
      LOGMSG_DEBUG_S(log_)<<"exception while resolving IP: "<<ex.what();
    }
  }
}

} // namespace DNSResolver
} // namespace Filter
