/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "Filter/SameName/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherHosts.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace SameName
{

Strategy::Strategy(unsigned int timeout):
  Filter::Simple::Strategy<Data>("samename", timeout)
{
}

Strategy::NodeEntry Strategy::makeThisEntry(const Node n) const
{
  return NodeEntry( n, Data() );
}

bool Strategy::isEntryInteresting(const NodeEntry /*thisEntry*/) const
{
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry /*thisEntry*/,
                                              const NodeEntry /*otherEntry*/) const
{
  // TODO: set meta alert name
  return "[samename] atacks from ...";
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // ok - both names are the same
  if(thisEntry.node_->getAlert().getName() == otherEntry.node_->getAlert().getName() )
    return true;

  return false;
}

} // namespace SameName
} // namespace Filter
