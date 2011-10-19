/*
 * GatherIPs.cpp
 *
 */
#include <cassert>

#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherIPs.hpp"

using namespace Persistency;


namespace Algo
{

namespace
{
struct FuncObj
{
  FuncObj(GatherIPs::IPSet &source, GatherIPs::IPSet &target):
    source_(&source),
    target_(&target)
  {
  }

  void operator()(const GraphNode *node)
  {
    assert(node!=NULL);
    assert( node->isLeaf() );
    addIPs( source_, node->getAlert()->getSourceHosts() );
    addIPs( target_, node->getAlert()->getTargetHosts() );
  }

private:
  void addIPs(GatherIPs::IPSet *out, const Persistency::Alert::Hosts &in)
  {
    for(Persistency::Alert::Hosts::const_iterator it=in.begin(); it!=in.end(); ++it)
    {
      const Persistency::Host::IP &ip=(*it)->getIP();
      assert(out!=NULL);
      ++(*out)[ip];     // increase counter for given IP by one
    } // for(hosts)
  }

  GatherIPs::IPSet *source_;
  GatherIPs::IPSet *target_;
}; // struct FuncObj
} // unnamed namespace


GatherIPs::GatherIPs(Persistency::ConstGraphNodePtrNN node)
{
  forEachUniqueLeaf( node.get(), (FuncObj(source_, target_)) );
}

GatherIPs::GatherIPs(const Persistency::GraphNode *node)
{
  forEachUniqueLeaf( node, (FuncObj(source_, target_)) );
}

} // namespace Algo
