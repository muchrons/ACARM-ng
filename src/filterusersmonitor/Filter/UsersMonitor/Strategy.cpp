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
  auto_ptr<Data> d=Data::createFrom( n->getAlert() );
  assert( d.get()!=NULL );
  assert( d->get().size()>0u );
  return *d;
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  assert( thisEntry.node_->isLeaf() && "ECL accepted non-leaf" );
  auto_ptr<Data> d=Data::createFrom( thisEntry.node_->getAlert() );
  if(d.get()==NULL)
    return false;
  assert( d->get().size()>0u );
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  Data::Names::const_iterator it=thisEntry.t_.commonWith(otherEntry.t_);
  assert( it!=thisEntry.t_.get().end() );
  stringstream ss;
  ss << "[usersmonitor] actions of user '"<<*it<<"'";
  return ss.str();
}


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  return thisEntry.t_.commonWith(otherEntry.t_)!=thisEntry.t_.get().end();
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       /*newNode*/) const
{
  Data::Names::const_iterator it=thisEntry.t_.commonWith(otherEntry.t_);
  assert( it!=thisEntry.t_.get().end() );
  return Data::createFrom(*it);
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace UsersMonitor
} // namespace Filter
