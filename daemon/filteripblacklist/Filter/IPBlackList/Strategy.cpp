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

Strategy::Strategy(const Parameters &params):
  Filter::Strategy<Data>("ipblacklist"),
  params_(params),
  deadline_(0)
{
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendProxy      &bp)
{
  //
  // this set of 'if's is important, since it prevents connection flood
  // when dshield host is not available. this makes processing nodes
  // much faster and therefor will not collect too many nodes in awaiting
  // queue in case of waiting for timeout.
  //

  const time_t now=time(NULL);
  // refresh list if timeout already passed.
  if(deadline_<now)
  {
    LOGMSG_INFO(log_, "updating IPs black list");
    deadline_=now+params_.refresh_;
    try
    {
      const Downloader    d(params_.limit_);
      const DShieldParser dsp( d.get() );
      BlackListPtr        ptr( new BlackList( dsp.begin(), dsp.end() ) );
      // if new list is available - save it!
      bl_.swap(ptr);
      LOGMSG_ERROR_S(log_)<<"update's done - next one on "<<deadline_;
    }
    catch(const Filter::Exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"Filter::Exception cought while downloading new "
                            "block list from dshiled: '"
                          <<ex.what()
                          <<"' - skipping until next refresh";
    } // catch()
  } // if(need_update)

  // if there is no list read, we can only skip this call...
  if( bl_.get()==NULL )
    return;

  assert( bl_.get()!=NULL );
  const EntryProcessor ep(bl_.get(), &bp, params_.priDelta_);
  Algo::forEachUniqueLeaf(n, ep);
}

} // namespace IPBlackList
} // namespace Filter
