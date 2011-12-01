/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMPLE_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SIMPLE_STRATEGY_HPP_FILE

/* public header */

#include <cassert>

#include "ConfigConsts/filters.hpp"
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
   *  \param type    type to assign.
   *  \param name    name to assign.
   *  \param timeout maximum time for node to be in timeout queue.
   */
  Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name, unsigned int timeout):
    Base(type, name),
    timeout_(timeout)
  {
  }

private:
  /** \brief creates NodeEntry form given node.
   *  \param n node to create entry for.
   *  \return user data for given node.
   */
  virtual TUserData makeThisEntryUserData(const Node n) const = 0;
  /** \brief checks if given entry is interesting for the filter on not.
   *  \param thisEntry entry to be checked.
   *  \return true if entry is interesting, false otherwise.
   */
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const = 0;
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
  /** \brief creates node entry for newly correlated node.
   *  \param thisEntry  entry created during this run.
   *  \param otherEntry entry saved after previous runs.
   *  \param newNode    newly correlated node.
   *  \return user data entry for correlated element.
   */
  virtual TUserData makeUserDataForNewNode(const NodeEntry &thisEntry,
                                           const NodeEntry &otherEntry,
                                           const Node       newNode) const = 0;
  /** \brief gives user a handle to postprocess new node.
   *  \param n  node to be post processed.
   *  \param bf backend facade to use during postprocessing.
   */
  virtual void postProcessNode(Node &n, BackendFacade &bf) const = 0;
  /** \brief gives user a handle to postprocess node (added after corraltion).
   *  \param entry node entry to be post processed.
   *  \param added newly added entry (i.e. added to 'entry' element).
   *  \param bf    backend facade to use during postprocessing.
   */
  virtual void postProcessNode(NodeEntry &entry, const NodeEntry &added, BackendFacade &bf) const = 0;


  // this is the core - do NOT overwrite this method
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf)
  {
    // prepare entry to compare with
    const NodeEntry thisEntry( n, makeThisEntryUserData(n) );
    assert( thisEntry.isSelfCorrelated()==false );
    // make a quick test, if this entry is interesting for a given filter at all
    if( !isEntryInteresting(thisEntry) )
      return;

    // check if it can be correlated with other nodes
    // (loop here ensures that correlation will proceed desipte of errors)
    int skip=0;
    for(;;)
    {
      // skip initial number of elements
      typename NodesTimeoutQueue::iterator begin=ntq.begin();
      for(int i=0; i<skip && begin!=ntq.end(); ++i)
        ++begin;

      const int ret=processLoopHelper(begin, thisEntry, ntq, bf);
      assert(ret!=0);
      if(ret<0)     // nothing more to be done.
        return;
      if(ret>0)     // error, so skip some of the elements in future
        skip+=ret;
    } // for(process_elements)
  }


  // implementation part of the loop. returns number of elements to skip, on
  // error or <0 to signal that processing has been done
  int processLoopHelper(typename NodesTimeoutQueue::iterator  begin,
                        const NodeEntry                      &thisEntry,
                        NodesTimeoutQueue                    &ntq,
                        BackendFacade                        &bf)
  {
    int skip=1;
    try
    {
      // check if it can be correlated with other nodes
      for(typename NodesTimeoutQueue::iterator it=begin; it!=ntq.end(); ++it)
      {
        // stop after first successful correlation.
        if( tryCorrelate(ntq, bf, thisEntry, it) )
          return -1;

        ++skip;                 // enlarge skip-elements count
      } // for(nodes in timeout queue)
    }
    catch(const Persistency::IO::Exception &ex)
    {
      // persistency-related exceptions are forwarded since they prohibit
      // later commision on transaction.
      LOGMSG_INFO_S(Base::log_) << "Persistency::IO exception caught (" << ex.getTypeName()
                                << ") - transaction is invalidated; forwarding exception";
      throw;  // this one's permanent in this context - re-throw
    }
    catch(const Commons::Exception &ex)
    {
      LOGMSG_INFO_S(Base::log_) << "exception (" << ex.getTypeName()
                                << ") has been thrown: '" << ex.what()
                                << "' - proceeding with processing next element";
      return skip;  // on error, continue with next element...
    }
    catch(const std::exception &ex)
    {
      LOGMSG_INFO_S(Base::log_) << "exception " << typeid(ex).name()
                                << " has been thrown: '"
                                << ex.what()
                                << "' - proceeding with processing next element";
      return skip;  // on error, continue with next element...
    }

    // if element cannot be correlated at the moment, add it to queue - maybe
    // we'll have better luck next time...
    ntq.update(thisEntry, getTimeoutForNotCorrelatedEntry(thisEntry) );
    return -2;
  }


  bool tryCorrelate(NodesTimeoutQueue                    &ntq,
                    BackendFacade                         &bf,
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

    // if node's leaf or is node from another filter, create new node and correlate nodes there.
    if( it->node_->isLeaf() || it->isSelfCorrelated()==false )
    {
      // create new meta-alert and one in queue
      LOGMSG_INFO_S(Base::log_)<< "correlating " << it->node_->getMetaAlert()->getID().get() << " ('"
                               << it->node_->getMetaAlert()->getName().get() << "') with "
                               << thisEntry.node_->getMetaAlert()->getID().get() << " ('"
                               << thisEntry.node_->getMetaAlert()->getName().get() << "') as a new node";
      const BackendFacade::ChildrenVector cv(it->node_, thisEntry.node_);
      const Persistency::MetaAlertPtrNN   ma(
                  new Persistency::MetaAlert( getMetaAlertName(thisEntry, *it),
                                              Persistency::MetaAlert::SeverityDelta(0),
                                              Persistency::MetaAlert::CertaintyDelta(0),
                                              Persistency::ReferenceURLPtr(),
                                              Persistency::Timestamp(),
                                              bf.getNextFreeID() ) );
      Persistency::GraphNodePtrNN newNode =bf.correlate(ma, cv);                                // add new, correlated element.
      postProcessNode(newNode, bf);
      const TUserData             userData=makeUserDataForNewNode(thisEntry, *it, newNode);     // create user's data
      const NodeEntry             newEntry=NodeEntry::makeCorrelatedEntry(newNode, userData);   // create correlated entry
      assert( newEntry.isSelfCorrelated()==true );
      ntq.dismiss(it);                    // if element has been already correlated
                                          // it should not be used any more.
      ntq.update(newEntry, getTimeout() );// add newly correlated entry.
    }
    else    // i.e.: is node
    {
      // append new meta-alert to a set of already correlated ones
      LOGMSG_INFO_S(Base::log_)<< "adding node " << thisEntry.node_->getMetaAlert()->getID().get() << " ('"
                               << thisEntry.node_->getMetaAlert()->getName().get() << "') to already correlated "
                               << it->node_->getMetaAlert()->getID().get() << " ('"
                               << it->node_->getMetaAlert()->getName().get() << "')";
      bf.addChild(it->node_, thisEntry.node_);      // add new alert to already correlated in one set
      postProcessNode(*it, thisEntry, bf);          // post-process node, after adding to correlation
    }

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

  unsigned int getTimeoutForNotCorrelatedEntry(const NodeEntry &ne) const
  {
    const time_t ct =ne.node_->getMetaAlert()->getCreateTime().get();
    const time_t now=time(NULL);
    // time is not properly synchronized?
    if(now<ct)
      return getTimeout();

    const unsigned int diff=now-ct;
    // return some reasonable timeout, if normal would be too short.
    if( diff>=getTimeout() )
      return std::min( getTimeout(), ConfigConsts::maxFilterQueueTimeoutForTooOld );

    // compute time in queue
    return getTimeout()-diff;
  }

  const unsigned int timeout_;
}; // class Strategy

} // namespace Simple
} // namespace Filter

#endif
