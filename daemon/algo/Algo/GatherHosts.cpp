/*
 * GatherHosts.cpp
 *
 */
#include <cassert>

#include "Algo/GatherHosts.hpp"


namespace Algo
{

GatherHosts::GatherHosts(Persistency::GraphNodePtrNN node):
  source_(new HostSet),
  target_(new HostSet)
{
  // gather data to collection. we can skip returned value since all instances
  // share the same shared pointers collection, therefore they're all saved
  // localy, discarding copying of actuall object's instance.
  Algo::forEachUniqueLeaf(node, *this);
}

void GatherHosts::operator()(Persistency::GraphNodePtrNN node)
{
  // assert is fine here, since this is implementation internal.
  assert( node->isLeaf() );
  addHosts( source_, node->getAlert()->getReportedSourceHosts() );
  addHosts( target_, node->getAlert()->getReportedTargetHosts() );
}

void GatherHosts::addHosts(HostSetPtr out, const Persistency::Alert::ReportedHosts &in)
{
  for(Persistency::Alert::ReportedHosts::const_iterator it=in.begin();
      it!=in.end(); ++it)
  {
    out->insert(*it);
  } // for(hosts)
}

} // namespace Algo
