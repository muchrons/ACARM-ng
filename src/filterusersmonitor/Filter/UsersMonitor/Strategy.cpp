/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Filter/UsersMonitor/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherHosts.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace UsersMonitor
{

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::Simple::Strategy<Data>("usersmonitor", name, timeout)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  // TODO
  return "TODO";            
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  // TODO
  return false;             
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  assert( thisEntry.t_==otherEntry.t_ );
  stringstream ss;
  ss << "[usersmonitor] actions of user '"<<thisEntry.t_<<"'";
  return ss.str();
}


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  return thisEntry.t_==otherEntry.t_;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &/*otherEntry*/,
                                      const Node       /*newNode*/) const
{
  return thisEntry.t_;
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace UsersMonitor
} // namespace Filter
