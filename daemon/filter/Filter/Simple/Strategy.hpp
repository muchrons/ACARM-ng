/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include "Filter/Strategy.hpp"


namespace Filter
{
namespace Simple
{

/** \brief basic filters common code template.
 *
 *  most filters are assumed to do very simillar things, like comparing
 *  new nodes with ones already held in queue and trying to find common
 *  correlation basing on filter-dependent rules.
 *
 *  this can be easily generated and minimized to set of basic functions that
 *  forwards calls to specific implementations, while keeping common parts
 *  separated.
 *
 *  in order to use this concretisation one must derive from this class and
 *  implementing all pure-virtual methods, but do NOT overwrite virtual
 *  members, that have been already defined.
 */
template<typename TUserData>
class Strategy: public Filter::Strategy<TUserData>
{
private:
  typedef Filter::Strategy<TUserData>      Base;

public:
  /** \brief helper forward of Base's type. */
  typedef typename Base::NodeEntry         NodeEntry;
  /** \brief helper forward of Base's type. */
  typedef typename Base::Node              Node;
  /** \brief helper forward of Base's type. */
  typedef typename Base::NodesTimeoutQueue NodesTimeoutQueue;

protected:
  /** \brief create instance.
   *  \param name    name to assign.
   *  \param timeout maximum time for node to be in timeout queue.
   */
  Strategy(const std::string &name, unsigned int timeout):
    Base(name),
    timeout_(timeout)
  {
  }

private:
  /** \brief creates NodeEntry form given node.
   *  \param n node to create entry from.
   *  \return node entry created from n.
   */
  virtual NodeEntry makeThisEntry(const Node n) const = 0;
  /** \brief checks if given entry is interesting for the filter on not.
   *  \param thisEntry entry to be checked.
   *  \return true if entry is interesting, false otherwise.
   */
  virtual bool isEntryInteresting(const NodeEntry &thisEntry) const = 0;
  /** \brief call gets name for correlated meta-alert for a given entries.
   *  \param thisEntry  entry created during this run.
   *  \param otherEntry entry that has been correlated with.
   *  \return name for the new, correlated meta-alert.
   */
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                    const NodeEntry thisEntry,
                                    const NodeEntry otherEntry) const = 0;
  /** \brief checks if given two entries can be correlated.
   *  \param thisEntry  entry created during this run.
   *  \param otherEntry entry saved after previous runs.
   *  \return true, if ientries are to be correlated, false otherwise.
   */
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const = 0;


  // this is the core - do NOT overwrite this method
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp)
  {
    // prepare entry to compare with
    const NodeEntry thisEntry( makeThisEntry(n) );
    // make a quick test, if this entry is interesting for a given filter at all
    if( !isEntryInteresting(thisEntry) )
      return;

    // check if it can be correlated with other nodes
    for(typename NodesTimeoutQueue::iterator it=ntq.begin(); it!=ntq.end(); ++it)
    {
      try
      {
        // stop after first successful correlation.
        if( tryCorrelate(ntq, bp, thisEntry, it) )
          return;
      }
      catch(const Commons::Exception &ex)
      {
        LOGMSG_INFO_S(Base::log_) << "exception (" << ex.getTypeName()
                                  << ") has been thrown: '" << ex.what()
                                  << "' - proceeding with processing next element";
        // on error, continue with next element...
      }
      catch(const std::exception &ex)
      {
        LOGMSG_INFO_S(Base::log_) << "exception (std::exception) has been thrown: '"
                                  << ex.what()
                                  << "' - proceeding with processing next element";
        // on error, continue with next element...
      }
    } // for(nodes in timeout queue)

    // if element cannot be correlated at the moment, add it to queue - maybe
    // we'll have better luck next time...
    ntq.update(thisEntry, getTimeout() );
  }

  bool tryCorrelate(NodesTimeoutQueue                    &ntq,
                    BackendProxy                         &bp,
                    const NodeEntry                      &thisEntry,
                    typename NodesTimeoutQueue::iterator  it)
  {
    // skip self
    if( *it==thisEntry )
      return false;
    // check if entries are to be correlated
    if( !canCorrelate(thisEntry, *it) )
      return false;

    // ok - we've got a match!

    // if node's leaf, create new node and correlate leafs there.
    if( it->node_->isLeaf() )
    {
      LOGMSG_DEBUG_S(StrategyBase::log_)
        << "correlating '" << it->node_->getMetaAlert()->getName().get()
        << "' with '" << thisEntry.node_->getMetaAlert().getName().get()
        << "' as a new node";
      const BackendProxy::ChildrenVector cv(it->node_, thisEntry.node_);
      const Persistency::MetaAlertPtrNN ma(
                  new Persistency::MetaAlert( getMetaAlertName(thisEntry, *it),
                                              Persistency::MetaAlert::SeverityDelta(0),
                                              Persistency::MetaAlert::CertaintyDelta(0),
                                              Persistency::ReferenceURLPtr(),
                                              Persistency::Timestamp() ) );
      // TODO: rework this in order to be able to continue correlating after
      //       removal of sample element.
      ntq.dismiss(it);                                // mark source element as already used
      Persistency::GraphNodePtrNN newNode=bp.correlate(ma, cv); // add new, correlated element
      const NodeEntry newEntry(newNode, thisEntry.t_);// use the same reported host entry
      ntq.update(newEntry, getTimeout() );            // add newly correlated entry
    }
    else
    {
      LOGMSG_DEBUG_S(StrategyBase::log_)
        << "adding node '" << thisEntry.node_->getMetaAlert().getName().get()
        << "' to already correlated '"
        << it->node_->getMetaAlert()->getName().get();
      bp.addChild(it->node_, thisEntry.node_);      // add new alert to already
    }                                               // correlated in one set

    // if we're here, it means that we were able to correlate and may exit
    // in glory now.
    // NOTE: we intentionaly skip adding new entry to queue, since it has
    //       been already correlated, therefore will not be used anymore.
    return true;
  }

  unsigned int getTimeout(void) const
  {
    return timeout_;
  }

  const unsigned int timeout_;
}; // class Strategy

} // namespace Simple
} // namespace Filter

#endif
