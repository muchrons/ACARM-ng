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

namespace
{
template<typename T>
void ignoreVariable(const T&)
{

}
}
namespace Filter
{
namespace SameName
{

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::Simple::Strategy<Data>("samename", name, timeout)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node /*n*/) const
{
  return Data();
}

bool Strategy::isEntryInteresting(const NodeEntry /*thisEntry*/) const
{
  // return true beacause meta-alert name is always not null
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // thisEntry and otherEntry must containt the same meta-alert name
  assert( canCorrelate(thisEntry, otherEntry) );
  ignoreVariable(otherEntry);
  stringstream ss;
  ss << "[samename] " << thisEntry.node_->getMetaAlert()->getName().get();
  return ss.str();
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // ok - both names are the same
  if( thisEntry.node_->getMetaAlert()->getName() == otherEntry.node_->getMetaAlert()->getName() )
    return true;
  return false;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &/*thisEntry*/,
                                      const NodeEntry &/*otherEntry*/,
                                      const Node       /*newNode*/) const
{
  return Data();
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace SameName
} // namespace Filter
