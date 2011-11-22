/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "ConfigConsts/inputs.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/IPBlackList/Strategy.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"
#include "Filter/IPBlackList/DShieldParser.hpp"
#include "Filter/IPBlackList/Downloader.hpp"
#include "Filter/IPBlackList/config.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;

namespace Filter
{
namespace IPBlackList
{

Strategy::Strategy(const Core::Types::Proc::InstanceName &name, const Parameters &params):
  Filter::Strategy<Data>( TypeName("ipblacklist"), name ),
  params_(params),
  nextPrune_(0),
  deadline_(0),
  dwnl_(params_.limit_)
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

  //
  // this set of 'if's is important, since it prevents connection flood
  // when dshield host is not available. this makes processing nodes
  // much faster and therefor will not collect too many nodes in awaiting
  // queue in case of waiting for timeout.
  //

  // refresh list if timeout already passed
  if(deadline_<now)
    updateBlackList(now, bf);
  assert(deadline_>=now);

  // if there is no list read, we can only skip this call...
  if( bl_.get()==NULL )
  {
    handleNoBlackList(now, n);
    assert(deadline_>=now);
    return;
  }

  // process given node
  assert( bl_.get()!=NULL );
  const EntryProcessor ep(bl_.get(), &bf, &processed_, params_.priDelta_);
  Algo::forEachUniqueLeaf(n, ep);
}

void Strategy::pruneProcessedSet(const time_t now)
{
  LOGMSG_DEBUG(log_, "prunning time has come");
  processed_.prune();   // call prune method
  nextPrune_=now+10;    // schedule prunning every 10[s]
  LOGMSG_DEBUG_S(log_)<<"next prunning scheduled on/after "<<nextPrune_;
}

void Strategy::updateBlackList(time_t now, BackendFacade &bf)
{
  LOGMSG_INFO(log_, "updating IPs black list");
  deadline_=now+params_.refresh_;
  try
  {
    LOGMSG_DEBUG(log_, "downloading IP black list");
    const DShieldParser dsp( dwnl_.download() );
    LOGMSG_DEBUG(log_, "parsing IP black list");
    BlackListPtr        ptr( new BlackList( dsp.begin(), dsp.end() ) );

    // if new list is available - save it!
    LOGMSG_DEBUG(log_, "applying new IP black list");
    bl_.swap(ptr);
    LOGMSG_INFO_S(log_)<<"update's done - next one on/after "<<deadline_;

    // if all's done, send heartbeat to signal dshield is alive
    bf.heartbeat("dshield.org", params_.refresh_+timeWindowForDownload);
    bf.commitChanges();
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
  const time_t waitTime=maxDelayBetweenDownloadRetries;
  if(deadline_>now+waitTime)
  {
    deadline_=now+waitTime;
    LOGMSG_INFO_S(log_)<<"next download is scheduled too far in the future thus resheduling "
                         " next download to be on/after "<<deadline_;
  }
}

} // namespace IPBlackList
} // namespace Filter
