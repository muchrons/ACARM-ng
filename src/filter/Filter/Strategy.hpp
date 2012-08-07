/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_STRATEGY_HPP_FILE

/* public header */

#include <sstream>
#include <ctime>
#include <cassert>
#include <boost/operators.hpp>

#include "System/ignore.hpp"
#include "Base/TimeoutQueue.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/StrategyBase.hpp"


namespace Filter
{

/** \brief interface (base) for all filters.
 */
template<typename T>
class Strategy: public StrategyBase
{
public:
  /** \brief helper typedef for GraphNode pointer. */
  typedef BackendFacade::Node           Node;
  /** \brief helper typedef for GraphNode pointer - const version. */
  typedef BackendFacade::ConstNode      ConstNode;
  /** \brief helper typedef for list of chenged nodes. */
  typedef BackendFacade::ChangedNodes   ChangedNodes;
  /** \brief forward declaration to keep code easier to read. */
  struct NodeEntry;
  /** \brief timeouting queue colleciton type. */
  typedef Base::TimeoutQueue<NodeEntry> NodesTimeoutQueue;

  /** \brief processes given meta-alert.
   *  \param n       node to be processed.
   *  \param changed output collection of changed nodes.
   */
  void process(Node n, ChangedNodes &changed)
  {
    LOGMSG_DEBUG_S(log_)<<"processing node "<< n->getMetaAlert()->getID().get();
    assert( changed.size()==0 && "non-empty output collection received");
    pruneNTQ();                 // clean queue's content.
    BackendFacade bf( conn_, changed, getFilterType(), getFilterName() );
    processImpl(n, ntq_, bf);
    bf.commitChanges();         // if there was no exception, commit changes made (if any)
    LOGMSG_DEBUG_S(log_)<<"nodes timeout queue size is "<<ntq_.size()<<" elements";
  }
  /** \brief send heartbeat for this module.
   *  \param deadline maximum ammount of time for heartbeat to arrive
   */
  void heartbeat(const unsigned int deadline)
  {
    ChangedNodes  changed;
    BackendFacade bf( conn_, changed, getFilterType(), getFilterName() );
    bf.heartbeat(deadline);
    bf.commitChanges();         // if there was no exception, commit changes made (if any)
    assert(changed.size()==0u);
  }

  /** \brief helper structure with user-provided data associated with node's entry.
   */
  struct NodeEntry: public boost::equality_comparable<NodeEntry>
  {
    /** \brief create entry object.
     *  \param node node to create.
     *  \param t    data to be associated with node.
     */
    NodeEntry(Node node, const T &t):
      node_(node),
      t_(t),
      isSelfCorrelated_(false)
    {
    }
    /** \brief named c-tor for creating correlated node.
     *  \param node node to create.
     *  \param t    data to be asociated with node.
     *  \return return correlated node.
     */
    static NodeEntry makeCorrelatedEntry(Node node, const T &t)
    {
      NodeEntry ne(node, t);
      ne.isSelfCorrelated_=true;
      return ne;
    }

    /** \brief check if given two entries correspond to the same node.
     *  \param other node to compare with.
     *  \return true if given entries correspond to the same node, false otherwise.
     */
    bool operator==(const NodeEntry &other) const
    {
      if(this==&other)
        return true;
      return node_.get()==other.node_.get();
    }
    /** \brief tells if entry has been correlated by this filter.
     *  \return true if this filter correlated this entry, false otherwise.
     */
    bool isSelfCorrelated(void) const
    {
      return isSelfCorrelated_;
    }

    Node node_;     ///< node itself.
    T    t_;        ///< user data, associated with node.

  private:
    bool isSelfCorrelated_; // true when this filter correlated this entry
  }; // struct NodeEntry

protected:
  /** \brief create instance.
   */
  Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name):
    StrategyBase(type, name),
    nextPrune_(0)
  {
  }

private:
  /** \brief user-provided implementation of node processing.
   *  \param n       added/changed node to be processed by filter.
   *  \param changed collection of nodes that changed (user should use this
   *                 to inform other filters about changes).
   *  \param ntq     internal filter's collection of elements with assigned
   *                 timeout. it can be browsed any time. if element is to be
   *                 saved for processing in later calls (ex.: it looks like it
   *                 can be correlated later on) it is to be saved here, with
   *                 proper timeout.
   *  \param bf      persistency facade - object that allows saving changes on
   *                 persistent storage.
   */
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf) = 0;

  void pruneNTQ(void)
  {
    // do this only once per some time
    const time_t now=time(NULL);
    if(nextPrune_>now)
      return;
    LOGMSG_DEBUG(log_, "pruning time has come");
    const size_t size=ntq_.size();
    const size_t pruned=ntq_.prune();   // do periodical queue's clean-up
    System::ignore(size);
    System::ignore(pruned);

    LOGMSG_DEBUG_S(log_)<<"pruned "<<pruned<<" of "<<size<<" elements";
    nextPrune_=now+1;                   // it does not make sense to make it more often than once per 1[s]
    LOGMSG_DEBUG_S(log_)<<"next pruning on "<<nextPrune_;
  }

  NodesTimeoutQueue ntq_;
  time_t            nextPrune_;
}; // class Strategy

} // namespace Filter

#endif
