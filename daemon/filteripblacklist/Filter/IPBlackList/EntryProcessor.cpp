/*
 * EntryProcessor.cpp
 *
 */
#include <cassert>

#include "Filter/IPBlackList/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace IPBlackList
{

EntryProcessor::EntryProcessor(const BlackList *bl,
                               BackendFacade   *bf,
                               ProcessedSet    *ps,
                               const double     priDelta):
  log_("filter.ipblacklist"),
  bl_(bl),
  bf_(bf),
  ps_(ps),
  priDelta_(priDelta)
{
  // this is only internal implementation, thus we can do this safely
  assert(bl_!=NULL);
  assert(bf_!=NULL);
  assert(ps_!=NULL);
}

void EntryProcessor::operator()(Persistency::GraphNodePtrNN leaf)
{
  assert( leaf->isLeaf() && "wrong graph-passing algorithm choosen" );
  LOGMSG_DEBUG(log_, "processing source hosts");
  processHosts(leaf, leaf->getAlert()->getReportedSourceHosts() );  // we may be under attack
  LOGMSG_DEBUG(log_, "processing target hosts");
  processHosts(leaf, leaf->getAlert()->getReportedTargetHosts() );  // zombie host may be contacting C&C
}

void EntryProcessor::processHosts(Persistency::GraphNodePtrNN              leaf,
                                  const Persistency::Alert::ReportedHosts &rh)
{
  assert( leaf->isLeaf() && "looks like called with wrong algorithm" );
  // process new entry
  typedef Persistency::Alert::ReportedHosts::const_iterator ConstIterator;
  for(ConstIterator it=rh.begin(); it!=rh.end(); ++it)
  {
    const HostPtrNN host=*it;               // helper object
    assert(bl_!=NULL);
    if( (*bl_)[ host->getIP() ] )           // is IP blacklisted?
    {
      assert(ps_!=NULL);
      if( ps_->isProcessed(leaf, host) )    // skip already checked host
      {
        LOGMSG_DEBUG_S(log_)<<"host "<<host->getIP()<<" from (meta-)alert "<<leaf->getMetaAlert()->getID().get()
                            <<" is blacklisted, but has been already processed - skipping";
        continue;
      }
      // new host from black list - increase priority of the (meta-)alert
      assert(bf_!=NULL);
      LOGMSG_INFO_S(log_)<<"host "<<host->getIP()<<" from (meta-)alert "<<leaf->getMetaAlert()->getID().get()
                         <<" is blacklisted - adding "<<priDelta_<<" to priority";
      bf_->updateSeverityDelta(leaf, priDelta_);
      ps_->markAsProcessed(leaf, host);     // mark change in cache to avoid doing it again.
    }
  } // for(all_hosts)
}

} // namespace IPBlackList
} // namespace Filter
