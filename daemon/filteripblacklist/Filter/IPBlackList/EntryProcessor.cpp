/*
 * EntryProcessor.cpp
 *
 */
#include "Filter/IPBlackList/EntryProcessor.hpp"

using namespace Persistency;

namespace Filter
{
namespace IPBlackList
{

EntryProcessor::EntryProcessor(const BlackList *bl,
                               BackendProxy    *bp,
                               const double     priDelta):
  bl_(bl),
  bp_(bp),
  priDelta_(priDelta)
{
  assert(bl_!=NULL);    // this is only internal implementation
  assert(bp_!=NULL);    // this is only internal implementation
}

void EntryProcessor::operator()(Persistency::GraphNodePtrNN leaf)
{
  assert( leaf->isLeaf() && "wrong graph-passing algorithm choosen" );
  processHosts(leaf, leaf->getAlert()->getReportedSourceHosts() );
  //processHosts(leaf, leaf->getAlert()->getReportedTargetHosts() );
}

void EntryProcessor::processHosts(Persistency::GraphNodePtrNN              leaf,
                                  const Persistency::Alert::ReportedHosts &rh)
{
  typedef Persistency::Alert::ReportedHosts::const_iterator ConstIterator;
  for(ConstIterator it=rh.begin(); it!=rh.end(); ++it)
  {
    const HostPtrNN host=*it;       // helper object
    if( (*bl_)[ host->getIP() ] )   // is IP blacklisted?
      bp_->updateSeverityDelta(leaf, priDelta_);
  } // for(all_hosts)
}

} // namespace IPBlackList
} // namespace Filter
