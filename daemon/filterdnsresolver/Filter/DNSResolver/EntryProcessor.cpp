/*
 * EntryProcessor.cpp
 *
 */
#include "Filter/DNSResolver/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace DNSResolver
{

EntryProcessor::EntryProcessor(CachedDNS *cache, BackendFacade *bf):
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
    if( host->getName()!=NULL )     // if host laready have DNS name skip it
      continue;

    // if no entry, try getting one
    const CachedDNS::Entry e=(*cache_)[ host->getIP() ];
    if(e.first)                                         // has name?
      bf_->setHostName(leaf, host, e.second.get() );    // set it!
  }
}

} // namespace DNSResolver
} // namespace Filter
