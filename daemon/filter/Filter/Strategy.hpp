/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_STRATEGY_HPP_FILE

/* public header */

#include <boost/operators.hpp>

#include "Base/TimeoutQueue.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendProxy.hpp"
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
  typedef BackendProxy::Node         Node;
  /** \brief helper typedef for list of chenged nodes. */
  typedef BackendProxy::ChangedNodes ChangedNodes;

  /** \brief processes given meta-alert.
   */
  void process(Node n, ChangedNodes &changed)
  {
    LOGMSG_DEBUG_S(log_)<<"processing node at address 0x"
                        <<static_cast<void*>( n.get() );
    BackendProxy bp( conn_, changed, getFilterName() );
    assert( changed.size()==0 && "non-empty output collection received");
    processImpl(n, ntq_, bp);
  }

protected:
  /** \brief helper structure with user-provided data associated with
   *         node's entry.
   */
  struct NodeEntry: public boost::equality_comparable<NodeEntry>
  {
    /** \brief create entry object.
     *  \param node node to create.
     *  \param t    data to be associated with node.
     */
    NodeEntry(Node node, const T &t):
      node_(node),
      t_(t)
    {
    }
    /** \brief check if given two entries correspond to the same node.
     *  \param other node to compare with.
     *  \return true if given entries correspond to the same node, false otherwise.
     */
    bool operator==(const NodeEntry &other) const
    {
      return node_.get()==other.node_.get();
    }

    Node node_;     ///< node itself.
    T    t_;        ///< user data, associated with node.
  };

  /** \brief timeouting queue colleciton type. */
  typedef Base::TimeoutQueue<NodeEntry> NodesTimeoutQueue;

  /** \brief create instance.
   */
  explicit Strategy(const std::string &name):
    StrategyBase(name)
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
   *  \param bp      persistency proxy - object that allows saving changes on
   *                 persistent storage.
   */
  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp) = 0;

  NodesTimeoutQueue ntq_;
}; // class Strategy

} // namespace Filter

#endif
