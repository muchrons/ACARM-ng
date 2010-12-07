/*
 * GatherIPs.cpp
 *
 */
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherIPs.hpp"

namespace Algo
{

GatherIPs::GatherIPs(Persistency::ConstGraphNodePtrNN node):
  source_(new IPSet),
  target_(new IPSet)
{
  // all members are held by pointer, thus copyies are equal and we can
  // pass selfs as argument
  forEachUniqueLeaf(node, *this);
}

void GatherIPs::operator()(Persistency::ConstGraphNodePtrNN node)
{
  assert( node->isLeaf() );
  addIPs( source_, node->getAlert()->getSourceHosts() );
  addIPs( target_, node->getAlert()->getTargetHosts() );
}

void GatherIPs::addIPs(IPSetPtr out, const Persistency::Alert::Hosts &in)
{
  for(Persistency::Alert::Hosts::const_iterator it=in.begin(); it!=in.end(); ++it)
  {
    const Persistency::Host::IP &ip=(*it)->getIP();
    ++(*out)[ip];   // increase counter for given IP by one
  } // for(hosts)
}

} // namespace Algo
