/*
 * Strategy.cpp
 *
 */
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/NewEvent/Strategy.hpp"
#include "Filter/NewEvent/EntryProcessor.hpp"


namespace Filter
{
namespace NewEvent
{

Strategy::Strategy(const std::string &name, const Parameters &params):
  Filter::Strategy<Data>("newevent", name),
  nextPrune_(0),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
    Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
      ecl.add("*input*");   // TODO: magic value
        return ecl;
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf)
{
  //TODO
  const time_t now=time(NULL);
  // ensure prunning once a while
  if(nextPrune_<now)
    pruneProcessedSet(now);
  assert(nextPrune_>=now);

  const EntryProcessor ep(&bf, &processed_, &timeouted_, params_);
  Algo::forEachUniqueLeaf(n, ep);

}

void Strategy::pruneProcessedSet(const time_t now)
{
  LOGMSG_DEBUG(log_, "prunning time has come");
  processed_.prune();   // call prune method
  nextPrune_=now+10;    // schedule prunning every 10[s]
  LOGMSG_DEBUG_S(log_)<<"next prunning scheduled on/after "<<nextPrune_;
}

void Strategy::pruneTimeoutedSet()
{
  // TODO
}
} // namespace NewEvent
} // namespace Filter
