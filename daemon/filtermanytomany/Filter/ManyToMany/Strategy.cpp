/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "Filter/ManyToMany/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/ManyToMany/GatherHosts.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace ManyToMany
{

Strategy::Strategy(void):
  Filter::Simple::Strategy<Data>("manytomany", 5*60)
{
}

Strategy::NodeEntry Strategy::makeThisEntry(const Node n) const
{
  return NodeEntry( n, Data() );
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  const GatherHosts gh(thisEntry.node_);
  return gh.getSourceHosts().size()>0 &&
         gh.getTargetHosts().size()>0;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry /*thisEntry*/,
                                              const NodeEntry /*otherEntry*/) const
{
  return "[many2many] atacks from multiple hosts on multiple hosts detected";
}


namespace
{
/** \brief helper that determines if collection created out of the
 *         reults assigned to this iterator would be empty or not.
 */
struct IntersectionOutputIterator
{
  IntersectionOutputIterator(void):
    empty_(true)
  {
  }

  void operator++(void)
  {
    // ignore this call
  }

  IntersectionOutputIterator &operator*(void)
  {
    // return ourselfs to make space for assignment operator.
    return *this;
  }

  inline const HostPtrNN &operator=(const HostPtrNN &other)
  {
    empty_=false;
    return other;
  }

  bool isEmpty(void) const
  {
    return empty_;
  }

private:
  bool empty_;
}; // struct IntersectionOutputIterator
} // unnamed namespace


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // compute unique hosts
  const GatherHosts ghThis (thisEntry.node_);
  const GatherHosts ghOther(otherEntry.node_);

  IntersectionOutputIterator tmp;
  // perform set intersection on source hosts
  tmp=set_intersection( ghThis.getSourceHosts().begin(),
                        ghThis.getSourceHosts().end(),
                        ghOther.getSourceHosts().begin(),
                        ghOther.getSourceHosts().end(),
                        IntersectionOutputIterator(),
                        GatherHosts::HostSWO() );
  if( tmp.isEmpty() )
      return false;

  // perform set intersection on target hosts
  tmp=set_intersection( ghThis.getTargetHosts().begin(),
                        ghThis.getTargetHosts().end(),
                        ghOther.getTargetHosts().begin(),
                        ghOther.getTargetHosts().end(),
                        IntersectionOutputIterator(),
                        GatherHosts::HostSWO() );
  if( tmp.isEmpty() )
      return false;

  // ok - both intersections are non-empty
  return true;
}

} // namespace ManyToMany
} // namespace Filter
