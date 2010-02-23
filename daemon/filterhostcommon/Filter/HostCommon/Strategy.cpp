/*
 * Strategy.cpp
 *
 */
#include "Filter/HostCommon/Strategy.hpp"
#include "Logger/Logger.hpp"

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
                           BackendProxy      &bp)
{
  // get host from this node.
  HostPtr h=getReportedHost(n);
  // if there are no interesting entries, simply skip it.
  if( h.get()==NULL )
    return;
  // prepare entry to compare with
  const NodeEntry thisEntry(n, Data(h) );

  // check if it can be correlated with other nodes
  for(NodesTimeoutQueue::iterator it=ntq.begin(); it!=ntq.end(); ++it)
  {
    try
    {
      // stop after first successful correlation.
      if( tryCorrelate(ntq, bp, thisEntry, it) )
        return;
    }
    catch(const Commons::Exception &ex)
    {
      LOGMSG_INFO_S(log_) << "exception ("
                          << ex.getTypeName()
                          << ") has been thrown: '"
                          << ex.what()
                          << "' - proceeding with processing next element";
      // on error, continue with next element...
    }
    catch(const std::exception &ex)
    {
      LOGMSG_INFO_S(log_) << "exception (std::exception) has been thrown: '"
                          << ex.what()
                          << "' - proceeding with processing next element";
      // on error, continue with next element...
    }
  } // for(nodes in timeout queue)

  // if element cannot be correlated at the moment, add it to queue - maybe
  // we'll have better luck next time...
  ntq.update(thisEntry, getTimeout() );
}

bool Strategy::tryCorrelate(NodesTimeoutQueue           &ntq,
                            BackendProxy                &bp,
                            const NodeEntry             &thisEntry,
                            NodesTimeoutQueue::iterator  it)
{
  // skip self
  if( *it==thisEntry )
    return false;
  // check if node represents report on the same host (by IP address)
  if( thisEntry.t_.host_->getIP()!=it->t_.host_->getIP() )
    return false;

  // ok - we've got a match!

  // if node's leaf, create new node and correlate leafs there.
  if( it->node_->isLeaf() )
  {
    const BackendProxy::ChildrenVector cv(it->node_, thisEntry.node_);
    const MetaAlertPtrNN ma( new MetaAlert( getMetaAlertName(thisEntry.t_.host_),
                                            MetaAlert::SeverityDelta(0),
                                            MetaAlert::CertaintyDelta(0),
                                            ReferenceURLPtr(),
                                            Timestamp() ) );
    ntq.dismiss(it);                                // mark source element as already used
    GraphNodePtrNN  newNode=bp.correlate(ma, cv);   // add new, correlated element
    const NodeEntry newEntry(newNode, thisEntry.t_);// use the same reported host entry
    ntq.update(newEntry, getTimeout() );            // add newly correlated entry
  }
  else
    bp.addChild(it->node_, thisEntry.node_);  // add new alert to already
                                              // correlated in one set

  // if we're here, it means that we were able to correlate and may exit
  // in glory now.
  // NOTE: we intentionaly skip adding new entry to queue, since it has
  //       been already correlated, therefore will not be used anymore.
  return true;
}

} // namespace HostCommon
} // namespace Filter
