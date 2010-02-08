/*
 * Strategy.cpp
 *
 */
#include "Filter/HostCommon/Strategy.hpp"

using namespace Persistency;

namespace Filter
{
namespace HostCommon
{

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::Strategy<Data>(name),
  timeout_(timeout)
{
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &/*bp*/)
{
  // get host from this node.
  HostPtr h=getReportedHost(n);
  // if there is no interesting entries, simply skip it.
  if( h.get()==NULL )
    return;
  // add entry to observing set
  const NodeEntry thisEntry(n, Data(h) );
  ntq.update(thisEntry, getTimeout() );

  // check if it can be correlated with other nodes
  for(NodesTimeoutQueue::iterator it=ntq.begin(); it!=ntq.end(); ++it)
  {
    // skip self, if found
    if( *it==thisEntry )
      continue;
    // TODO
  }
}

} // namespace HostCommon
} // namespace Filter
