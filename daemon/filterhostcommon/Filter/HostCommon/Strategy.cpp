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

Strategy::Strategy(const std::string &name):
  Filter::Strategy<detail::Data>(name)
{
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendProxy      &/*bp*/)
{
  // get host from this node.
  HostPtr h=getReportedHost(n);
  // if there is no interesting entries, simply skip it.
  if( h.get()==NULL )
    return;
  // add entry to observing set
  //ntq.update( Data(
  // TODO
}

} // namespace HostCommon
} // namespace Filter
