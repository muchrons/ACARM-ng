/*
 * Strategy.cpp
 *
 */
#include "ConfigConsts/inputs.hpp"
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
  ecl.add( TypeName(ConfigConsts::defaultInputTypeName) );
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

  EntryProcessor ep(bf, processed_, timeouted_, params_);
  ep(n);
  pruneTimeoutedSet(bf);
}

void Strategy::pruneProcessedSet(const time_t now)
{
  LOGMSG_DEBUG(log_, "prunning time has come");
  processed_.prune();   // call prune method
  nextPrune_=now + params_.pruneTimeout_;    // schedule next prune
  LOGMSG_DEBUG_S(log_)<<"next prunning scheduled on/after "<<nextPrune_;
}

void Strategy::pruneTimeoutedSet(BackendFacade &bf)
{
  const Persistency::IO::DynamicConfig::Owner owner("Filter::NewEvent");
  timeouted_.markRemoved(bf, owner);
}

} // namespace NewEvent
} // namespace Filter
