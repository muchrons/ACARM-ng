/*
 * Strategy.cpp
 *
 */
#include <sstream>
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
  // TODO: meta-alert, not alert
  // return true beacause Alert name is always not null
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // thisEntry and otherEntry must containt the same Alert name
  assert( canCorrelate(thisEntry, otherEntry) );
  stringstream ss;
  // TODO: notice that you correlate m-as with with same name, so description
  //       is now redundant - should be '[samename] <...getName()...>'.
  ss << "[samename] this entry: " << thisEntry.node_->getMetaAlert().getName().get()
     << ", other entry: " << otherEntry.node_->getMetaAlert().getName().get();
  return ss.str();
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // ok - both names are the same
  if( thisEntry.node_->getMetaAlert().getName() == otherEntry.node_->getMetaAlert().getName() )
    return true;
  return false;
}

} // namespace SameName
} // namespace Filter
