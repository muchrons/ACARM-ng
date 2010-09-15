/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/IPBlackList/Strategy.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"
#include "Filter/IPBlackList/DShieldParser.hpp"
#include "Filter/IPBlackList/Downloader.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace IPBlackList
{

Strategy::Strategy(const std::string &name, const Parameters &params):
  Filter::Strategy<Data>("ipblacklist", name),
  params_(params),
  nextPrune_(0),
  deadline_(0),
  dwnl_(params_.limit_)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
  return ecl;
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendFacade     &bf)
{
  const time_t now=time(NULL);
  // ensure prunning once a while
  pruneIfNeeded(now);

  //
  // this set of 'if's is important, since it prevents connection flood
  // when dshield host is not available. this makes processing nodes
  // much faster and therefor will not collect too many nodes in awaiting
  // queue in case of waiting for timeout.
  //

  // refresh list if timeout already passed
  if(deadline_<now)
    updateBlackList(now);

  // if there is no list read, we can only skip this call...
  if( bl_.get()==NULL )
  {
    handleNoBlackList(now, n);
    return;
  }

  // process given node
  assert( bl_.get()!=NULL );
  const EntryProcessor ep(bl_.get(), &bf, &processed_, params_.priDelta_);
  Algo::forEachUniqueLeaf(n, ep);
}

void Strategy::pruneIfNeeded(const time_t now)
{
  // time has not come yet?
  if(nextPrune_>now)
    return;
  LOGMSG_DEBUG(log_, "prunning time has come");
  processed_.prune();   // call prune method
  nextPrune_=now+10;    // schedule prunning every 10[s]
  LOGMSG_DEBUG_S(log_)<<"next prunning scheduled on/after "<<nextPrune_;
}

void Strategy::updateBlackList(time_t now)
{
  LOGMSG_INFO(log_, "updating IPs black list");
  deadline_=now+params_.refresh_;
  try
  {
    const DShieldParser dsp( dwnl_.download() );
    BlackListPtr        ptr( new BlackList( dsp.begin(), dsp.end() ) );
    // if new list is available - save it!
    bl_.swap(ptr);
    LOGMSG_INFO_S(log_)<<"update's done - next one on/after "<<deadline_;
  }
  catch(const Filter::Exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"Filter::Exception caught while downloading new "
                          "block list from dshiled: '"
                        <<ex.what()
                        <<"' - skipping until next refresh...";
  } // catch()
}

void Strategy::handleNoBlackList(time_t now, Node n)
{
  LOGMSG_WARN_S(log_)<<"no blacklist is present - skipping scan for node "
                     <<n->getMetaAlert()->getID().get();
  // check if rescheduling download to be sooner is a good idea
  const time_t waitTime=123;
  if(deadline_>now+waitTime)
  {
    deadline_=now+waitTime;
    LOGMSG_INFO_S(log_)<<"next download is scheduled too far in the future thus resheduling "
                         " next download to be on/after "<<deadline_;
  }
}

} // namespace IPBlackList
} // namespace Filter
