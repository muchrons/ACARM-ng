/*
 * Strategy.cpp
 *
 */
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/NewEvent/Strategy.hpp"
#include "Filter/NewEvent/EntryProcessor.hpp"

using namespace Core::Types::Proc;

namespace Filter
{
namespace NewEvent
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Parameters &params):
  Filter::Strategy<Data>( TypeName("newevent"), name),
  nextPrune_(0),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add( TypeName("*input*") );   // TODO: magic value
  return ecl;
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendFacade     &bf)
{
  const time_t now=time(NULL);
  // ensure prunning once a while
  if(nextPrune_<now)
    pruneProcessedSet(now);
  assert(nextPrune_>=now);

  const EntryProcessor ep(bf, processed_, timeouted_, params_);
  // TODO: notice that ECL allows ownly alerts (i.e. leafs), thus 'ep' can be directlly applied
  //       to an 'n' object. assert on it being a leaf can be added to be sure
  Algo::forEachUniqueLeaf(n, ep);
  pruneTimeoutedSet(bf);
}

void Strategy::pruneProcessedSet(const time_t now)
{
  LOGMSG_DEBUG(log_, "prunning time has come");
  processed_.prune();   // call prune method
  // TODO: 10 is hardcoded value; move it to Parameters, this can be then entered as a predefined
  //       value when building from Builder. by doing so you can replace it in tests with for example 1
  //       this, making some tests run 10x faster.
  nextPrune_=now+10;    // schedule prunning every 10[s]
  LOGMSG_DEBUG_S(log_)<<"next prunning scheduled on/after "<<nextPrune_;
}

void Strategy::pruneTimeoutedSet(BackendFacade &bf)
{
  const Persistency::IO::DynamicConfig::Owner owner("Filter::NewEvent");
  timeouted_.markRemoved(bf, owner);
}

} // namespace NewEvent
} // namespace Filter
