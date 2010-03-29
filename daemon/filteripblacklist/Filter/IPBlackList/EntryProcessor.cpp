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
                               const double     priDelta):
  bl_(bl),
  bf_(bf),
  priDelta_(priDelta)
{
  assert(bl_!=NULL);    // this is only internal implementation
  assert(bf_!=NULL);    // this is only internal implementation
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
    assert(bl_!=NULL);
    if( (*bl_)[ host->getIP() ] )   // is IP blacklisted?
    {
      assert(bf_!=NULL);
      bf_->updateSeverityDelta(leaf, priDelta_);
    }
  } // for(all_hosts)
}

} // namespace IPBlackList
} // namespace Filter
