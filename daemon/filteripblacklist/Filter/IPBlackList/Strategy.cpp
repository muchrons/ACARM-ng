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
  const time_t now=time(NULL);
  // refresh list if timeout already passed.
  if( deadline_<now || bl_.get()==NULL )
  {
    deadline_=now+params_.refresh_;
    try
    {
      const Downloader    d(params_.limit_);
      const DShieldParser dsp( d.get() );

      BlackListPtr ptr( new BlackList( dsp.begin(), dsp.end() ) );
      bl_.swap(ptr);
    }
    catch(const Filter::Exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"Filter::Exception cought while downloading new "
                            "block list from dshiled: '"
                          <<ex.what()
                          <<"' - skipping until next refresh";
      // if there is no list read, we can only skip this call...
      if( bl_.get()==NULL )
      {
        LOGMSG_INFO(log_, "no black list present - skipping this run...");
        return;
      }
    } // catch()
  } // if(need_update)

  assert( bl_.get()!=NULL );
  const EntryProcessor ep(bl_.get(), &bp, params_.priDelta_);
  Algo::forEachUniqueLeaf(n, ep);
}

} // namespace IPBlackList
} // namespace Filter
