/*
 * Strategy.cpp
 *
 */
#include <algorithm>
#include <cassert>

#include "Filter/EventChain/Strategy.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace EventChain
{

Strategy::Params::Params(unsigned int timeout, double priDelta_):
  timeout_(timeout),
  priDelta_(priDelta_)
{
}

Strategy::Strategy(const std::string &name, const Params &params):
  Filter::Simple::Strategy<Data>("eventchain", name, params.timeout_),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  // accept ALL events on input
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultAccept();
  return ecl;

  // TODO: remove this code?
  //Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  //ecl.add("*input*");   // TODO: magic value
}

Data Strategy::makeThisEntryUserData(const Node n) const
{
  // single node does not represent chain
  Data d;
  Algo::GatherIPs ips(n);
  d.begin_=boost::shared_ptr<Algo::GatherIPs>( new Algo::GatherIPs::IPSet( ips.getSourceIPs() ) );
  d.end_  =boost::shared_ptr<Algo::GatherIPs>( new Algo::GatherIPs::IPSet( ips.getTargetIPs() ) );
}

bool Strategy::isEntryInteresting(const NodeEntry thisEntry) const
{
  const Algo::GatherIPs gip(thisEntry.node_);
  return gip.getSourceIPs().size()>0 &&
         gip.getTargetIPs().size()>0;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry /*thisEntry*/,
                                              const NodeEntry /*otherEntry*/) const
{
  return "[eventchain] chain of events detected";
}


bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanity check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );

  // compute IPs counts
  const Algo::GatherIPs gipThis (thisEntry.node_);
  const Algo::GatherIPs gipOther(otherEntry.node_);

  // TODO

  return false;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       /*newNode*/) const
{
  // TODO
  return Data();
}

} // namespace EventChain
} // namespace Filter
