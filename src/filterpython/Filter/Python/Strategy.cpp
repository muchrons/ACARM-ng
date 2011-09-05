/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Filter/Python/Strategy.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;


namespace Filter
{
namespace Python
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Config &cfg):
  Filter::Simple::Strategy<DataPtr>( TypeName("python"), name, cfg.getTimeout())
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName("*input*") );   // TODO: magic value
  return ecl;
}

DataPtr Strategy::makeThisEntryUserData(const Node /*n*/) const
{
  // TODO
  return DataPtr();
}

bool Strategy::isEntryInteresting(const NodeEntry /*thisEntry*/) const
{
  // TODO
  // return true beacause meta-alert name is always not null
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // TODO
  // thisEntry and otherEntry must containt the same meta-alert name
  assert( canCorrelate(thisEntry, otherEntry) );
  stringstream ss;
  ss << "[python]";
  return ss.str();
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // TODO
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  return false;
}

DataPtr Strategy::makeUserDataForNewNode(const NodeEntry &/*thisEntry*/,
                                         const NodeEntry &/*otherEntry*/,
                                         const Node       /*newNode*/) const
{
  // TODO
  return DataPtr();
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // TODO
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // TODO
  // nothing to be done here
}

} // namespace Python
} // namespace Filter
